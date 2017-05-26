#include "ShaderLoader.h"
#include "Constants.h"
#include "MathExt.h"

#if !defined(_WIN32) && !defined(__APPLE__)
#include <SDL2/SDL_image.h>
#elif defined(__APPLE__)
#include <SDL2_image/SDL_image.h>
#else
#include "Windows.h"
#include <GL\glew.h>
#include "SDL_image.h"
#undef LoadImage
#undef SendMessage
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#endif

ShaderLoader::ShaderLoader()
{
}

ShaderLoader::~ShaderLoader()
{
}

/**
 * @brief Loads shaders
 * @param aVertexShaderFilename Name of vertex shader.
 * @param aFragmentShaderFilename Name of fragment shader.
 * @return Shader data.
 */
ShaderData* ShaderLoader::LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename)
{
  ShaderData* ret = new ShaderData();
  std::ifstream vertexFile(Common::RelativePath("Shaders", aVertexShaderFilename.ToCharArray()).c_str());
  std::ifstream fragmentFile(Common::RelativePath("Shaders", aFragmentShaderFilename.ToCharArray()).c_str());
  if(vertexFile.is_open() && fragmentFile.is_open())
  {
    GLuint program = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLint didCompile = 0;
    GLint isLinked = 0;
    
    HashString vertexFileContents = std::string((std::istreambuf_iterator<char>(vertexFile)), std::istreambuf_iterator<char>());
    const char* vertexContents = vertexFileContents.ToCharArray();
    
    // Compile
    glShaderSource(vertexShader, 1, &vertexContents, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &didCompile);
    if(didCompile == GL_FALSE)
    {
      DebugLogPrint("VERTEX SHADER %s READ:\n%s\n", aVertexShaderFilename.ToCharArray(), vertexContents);
      
      GLint maxLength = 0;
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

      //The maxLength includes the NULL character
      std::vector<char> infoLog(maxLength);
      glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
      
      DebugLogPrint("GLERROR %s: %s\n", aVertexShaderFilename.ToCharArray(), &infoLog[0]);
	
      //We don't need the shader anymore.
      glDeleteShader(vertexShader);
    }
    
    HashString fragmentFileContents = std::string((std::istreambuf_iterator<char>(fragmentFile)), std::istreambuf_iterator<char>());
    const char* fragmentContents = fragmentFileContents.ToCharArray();
    glShaderSource(fragmentShader, 1, &fragmentContents, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &didCompile);
    if(didCompile == GL_FALSE)
    {
      DebugLogPrint("FRAGMENT SHADER %s READ:\n%s\n", aFragmentShaderFilename.ToCharArray(), fragmentContents);
      
      GLint maxLength = 0;
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

      //The maxLength includes the NULL character
      std::vector<char> infoLog(maxLength);
      glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
      
      DebugLogPrint("GLERROR %s: %s\n", aFragmentShaderFilename.ToCharArray(), &infoLog[0]);
	
      //We don't need the shader anymore.
      glDeleteShader(fragmentShader);
    }
    
    // Linking
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

      //The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        
      //We don't need the program anymore.
      glDeleteProgram(program);
      //Don't leak shaders either.
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      DebugLogPrint("GL LINK ERROR: %s\n", &infoLog[0]);
    }
    
    ret->mProgramID = program;
    ret->mVertexShaderID = vertexShader;
    ret->mFragmentShaderID = fragmentShader;
    ret->mVertexContents = vertexContents;
    ret->mFragmentContents = fragmentContents;
    ret->mVertexFileName = aVertexShaderFilename;
    ret->mFragmentFileName = aFragmentShaderFilename;
  }
  else
  {
    DebugLogPrint("No shader with name %s or %s found\n", aVertexShaderFilename.ToCharArray(), aFragmentShaderFilename.ToCharArray());
    assert(!"Shader file not found.");
  }
  
  return ret;
}

/**
 * @brief Load texture.
 * @param aTextureFileName Name of texture file.
 * @return Data for texture.
 */
TextureData* ShaderLoader::LoadTexture(HashString const &aTextureFileName)
{
  TextureData* textureData = new TextureData();
  GLenum textureFormat;
  SDL_Surface* surface = IMG_Load(Common::RelativePath("Art", aTextureFileName).c_str());
  int numberOfColors = 0;
  if(surface)
  {
    if ((surface->w & (surface->w - 1)) != 0 )
    {
      DebugLogPrint("warning: width of image: %s is not a power of 2\n", aTextureFileName.ToCharArray());
    }

    if ((surface->h & (surface->h - 1)) != 0 )
    {
      DebugLogPrint("warning: height of image: %s is not a power of 2\n", aTextureFileName.ToCharArray());
    }
    
    numberOfColors = surface->format->BytesPerPixel;
    if (numberOfColors == 4)
    {
      if (surface->format->Rmask == 0x000000ff)
        textureFormat = GL_RGBA;
      else
        #ifndef _WIN32
          textureFormat = GL_BGRA;
        #else
          textureFormat = GL_RGBA;
        #endif
    }
    else if (numberOfColors == 3)
    {
      if (surface->format->Rmask == 0x000000ff)
        textureFormat = GL_RGB;
      else
        #ifndef _WIN32
          textureFormat = GL_BGR;
        #else
          textureFormat = GL_RGB;
        #endif
    }
    else
    {
      DebugLogPrint("warning: image %s is not truecolor...  this will probably break\n", aTextureFileName.ToCharArray());
      DebugLogPrint("warning: bytes per pixel for image %s: %d\n", aTextureFileName.ToCharArray(), numberOfColors);
    }

    textureData->mTextureName = aTextureFileName;
    textureData->mWidth = surface->w;
    textureData->mHeight = surface->h;
    textureData->mTextureID = ImportTexture(surface, textureFormat);
    return textureData;
  }
  else
  {
    delete textureData;
    DebugLogPrint("warning: file: %s not found or incompatible format, check this out\n", aTextureFileName.ToCharArray());
    return nullptr;
  }
}

/**
 * @brief Load image as text.
 * @param aFont Font.
 * @param aText Contents.
 * @param aForegroundColor Primary color.
 * @param aBackgroundColor Background color.
 * @param aSize Size of font.
 * @param aMaxWidth Word wrapping.
 * @return Text as image.
 */
TextureData* ShaderLoader::LoadText(HashString const &aFont, HashString const &aText, Vector4 const &aForegroundColor, Vector4 const &aBackgroundColor, int aSize, int aMaxWidth)
{
  TextureData* textureData = new TextureData();
  GLenum textureFormat;
  Uint32 rmask, gmask, bmask, amask;
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
      rmask = 0xff000000;
      gmask = 0x00ff0000;
      bmask = 0x0000ff00;
      amask = 0x000000ff;
  #else
      rmask = 0x000000ff;
      gmask = 0x0000ff00;
      bmask = 0x00ff0000;
      amask = 0xff000000;
  #endif
  if(!TTF_WasInit())
    TTF_Init();
  TTF_Font* font = TTF_OpenFont(Common::RelativePath("Fonts", aFont).c_str(), aSize);
  if(!font)
  {
    delete textureData;
    font = nullptr;
    DebugLogPrint("warning: file not found or incompatible format, check this out\n");
    DebugLogPrint("%s", TTF_GetError());
    return nullptr;
  }

  // Create text texture
  SDL_Color fgColor = {(Uint8)aForegroundColor.x, (Uint8)aForegroundColor.y, (Uint8)aForegroundColor.z, (Uint8)aForegroundColor.w};
  //SDL_Color bgColor = {(Uint8)aBackgroundColor.x, (Uint8)aBackgroundColor.y, (Uint8)aBackgroundColor.z, (Uint8)aBackgroundColor.w};
  SDL_Surface *msg = TTF_RenderText_Blended_Wrapped(font, aText.ToCharArray(), fgColor, aMaxWidth);
  if(!msg)
  {
    DebugLogPrint("TTF_RenderText failed: %s", TTF_GetError());
    assert(msg);
  }

#ifdef __APPLE__
  textureFormat = GL_BGRA;
#else
  textureFormat = GL_RGBA;
#endif
  
  SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, msg->w, msg->h, 32, rmask, gmask, bmask, amask);
  SDL_BlitSurface(msg, NULL, surface, NULL);
  
  textureData->mTextureName = aFont + aText + Common::IntToString(aSize);
  textureData->mWidth = surface->w;
  textureData->mHeight = surface->h;
  textureData->mTextureID = ImportTexture(surface, textureFormat);
  TTF_CloseFont(font);
  return textureData;
}

/**
 * @brief Import texture into OpenGL.
 * @param aSurface Copy data from here.
 * @param aTextureFormat Format of image.
 * @return Texture id.
 */
int ShaderLoader::ImportTexture(SDL_Surface* aSurface, GLenum aTextureFormat)
{
  GLuint textureId = 0;
  GLint minFilter = GL_LINEAR;
  GLint magFilter = GL_LINEAR;
  GLint wrapS = GL_REPEAT;
  GLint wrapT = GL_REPEAT;
  if(Constants::GetString("OpenGLMinFilter") == "GL_NEAREST")
  {
    minFilter = GL_NEAREST;
  }
  if(Constants::GetString("OpenGLMagFilter") == "GL_NEAREST")
  {
    magFilter = GL_NEAREST;
  }
  if(Constants::GetString("OpenGLWrapModeS") == "GL_CLAMP_TO_EDGE")
  {
    wrapS = GL_CLAMP_TO_EDGE;
  }
  if(Constants::GetString("OpenGLWrapModeT") == "GL_CLAMP_TO_EDGE")
  {
    wrapT = GL_CLAMP_TO_EDGE;
  }

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
#ifdef __APPLE__
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_STORAGE_HINT_APPLE, GL_STORAGE_CACHED_APPLE);
  glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_TRUE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aSurface->w, aSurface->h, 0, aTextureFormat, GL_UNSIGNED_INT_8_8_8_8_REV, aSurface->pixels);
#else
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aSurface->w, aSurface->h, 0, aTextureFormat, GL_UNSIGNED_BYTE, aSurface->pixels);
#endif
  SDL_FreeSurface(aSurface);

  return textureId;
}
