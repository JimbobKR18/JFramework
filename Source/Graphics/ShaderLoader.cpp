#include "ShaderLoader.h"
#include "SystemProperties.h"
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

std::vector<SDL_Surface*> ShaderLoader::mSurfaces;
HashString const GLTextureInfo::GL_NEAREST_STRING = "GL_NEAREST";
HashString const GLTextureInfo::GL_NEAREST_MIPMAP_NEAREST_STRING = "GL_NEAREST_MIPMAP_NEAREST";
HashString const GLTextureInfo::GL_NEAREST_MIPMAP_LINEAR_STRING = "GL_NEAREST_MIPMAP_LINEAR";
HashString const GLTextureInfo::GL_LINEAR_MIPMAP_NEAREST_STRING = "GL_LINEAR_MIPMAP_NEAREST";
HashString const GLTextureInfo::GL_LINEAR_MIPMAP_LINEAR_STRING = "GL_LINEAR_MIPMAP_LINEAR";
HashString const GLTextureInfo::GL_CLAMP_TO_EDGE_STRING = "GL_CLAMP_TO_EDGE";
HashString const GLTextureInfo::GL_CLAMP_TO_BORDER_STRING = "GL_CLAMP_TO_BORDER";

//------------------------------
// GLTextureInfo
//------------------------------
GLTextureInfo::GLTextureInfo() : mTextureID(0), mMinFilter(GL_LINEAR), mMagFilter(GL_LINEAR), mWrapS(GL_REPEAT), mWrapT(GL_REPEAT)
{
}

GLTextureInfo::GLTextureInfo(HashString const &aMinFilter, HashString const &aMagFilter) : 
  mTextureID(0), mMinFilter(GL_LINEAR), mMagFilter(GL_LINEAR), mWrapS(GL_REPEAT), mWrapT(GL_REPEAT)
{
  Set(aMinFilter, aMagFilter);
}

/**
 * @brief Set filters
 * @param aMinFilter
 * @param aMagFilter
 */
void GLTextureInfo::Set(HashString const &aMinFilter, HashString const &aMagFilter)
{
  if(aMinFilter == GL_NEAREST_STRING)
  {
    mMinFilter = GL_NEAREST;
  }
  else if(aMinFilter == GL_NEAREST_MIPMAP_NEAREST_STRING)
  {
    mMinFilter = GL_NEAREST_MIPMAP_NEAREST;
  }
  else if(aMinFilter == GL_NEAREST_MIPMAP_LINEAR_STRING)
  {
    mMinFilter = GL_NEAREST_MIPMAP_LINEAR;
  }
  else if(aMinFilter == GL_LINEAR_MIPMAP_NEAREST_STRING)
  {
    mMinFilter = GL_LINEAR_MIPMAP_NEAREST;
  }
  else if(aMinFilter == GL_LINEAR_MIPMAP_LINEAR_STRING)
  {
    mMinFilter = GL_LINEAR_MIPMAP_LINEAR;
  }
  if(aMagFilter == GL_NEAREST_STRING)
  {
    mMagFilter = GL_NEAREST;
  }
  if(SystemProperties::GetWrapModeS() == GL_CLAMP_TO_EDGE_STRING)
  {
    mWrapS = GL_CLAMP_TO_EDGE;
  }
  if(SystemProperties::GetWrapModeT() == GL_CLAMP_TO_EDGE_STRING)
  {
    mWrapT = GL_CLAMP_TO_EDGE;
  }
  if(SystemProperties::GetWrapModeS() == GL_CLAMP_TO_BORDER_STRING)
  {
    mWrapS = GL_CLAMP_TO_BORDER;
  }
  if(SystemProperties::GetWrapModeT() == GL_CLAMP_TO_BORDER_STRING)
  {
    mWrapT = GL_CLAMP_TO_BORDER;
  }
}

//------------------------------
// ShaderLoader
//------------------------------
ShaderLoader::ShaderLoader()
{
}

ShaderLoader::~ShaderLoader()
{
}

/**
 * @brief Logs errors related to graphics.
 * @param aFile Name of file.
 * @param aLine Number of line.
 */
void ShaderLoader::LogError(HashString const &aFile, int aLine)
{
  GLenum glErr = glGetError();
  if (glErr != GL_NO_ERROR)
  {
    DebugLogPrint("glError in file %s @ line %d: %s\n", aFile.ToCharArray(), aLine, gluErrorString(glErr));
    if(glErr == GL_INVALID_FRAMEBUFFER_OPERATION)
    {
      GLenum framebufferErr = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      DebugLogPrint("Framebuffer error %d\n", framebufferErr);
    }
    //assert(!"GL_ERROR has occurred");
  }
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
 * @param aMinFilter Min filter.
 * @param aMagFilter Mag filter.
 * @return Data for texture.
 */
TextureData* ShaderLoader::LoadTexture(HashString const &aTextureFileName, HashString const &aMinFilter, HashString const &aMagFilter)
{
  TextureData* textureData = new TextureData();
  GLenum textureFormat = GL_RGBA;
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
    textureData->mSize = Vector2(surface->w, surface->h);
    textureData->mTextureID = ImportTexture(surface, textureFormat, aMinFilter, aMagFilter);
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
 * @param aMinFilter Min filter.
 * @param aMagFilter Mag filter.
 * @param aForegroundColor Primary color.
 * @param aBackgroundColor Background color.
 * @param aSize Size of font.
 * @param aMaxWidth Word wrapping.
 * @return Text as image.
 */
TextureData* ShaderLoader::LoadText(HashString const &aFont, HashString const &aText, HashString const &aMinFilter, 
  HashString const &aMagFilter, Vector4 const &aForegroundColor, Vector4 const &aBackgroundColor, int aSize, int aMaxWidth)
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
    DebugLogPrint("%s", TTF_GetError());
    return nullptr;
  }

  // Create text texture
  Vector4 foregroundColor = aForegroundColor * 255.0f;
  //Vector4 backgroundColor = aBackgroundColor * 255.0f;
  SDL_Color fgColor = {(Uint8)foregroundColor.x, (Uint8)foregroundColor.y, (Uint8)foregroundColor.z, (Uint8)foregroundColor.w};
  //SDL_Color bgColor = {(Uint8)backgroundColor.x, (Uint8)backgroundColor.y, (Uint8)backgroundColor.z, (Uint8)backgroundColor.w};
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
  
  textureData->mTextureName = aFont + aText + Common::IntToString(aSize) + aMinFilter + aMagFilter;
  textureData->mSize = Vector2(surface->w, surface->h);
  textureData->mTextureID = ImportTexture(surface, textureFormat, aMinFilter, aMagFilter);
  textureData->mFont = aFont;
  textureData->mText = aText;
  textureData->mForegroundColor = aForegroundColor;
  textureData->mBackgroundColor = aBackgroundColor;
  textureData->mFontSize = aSize;
  textureData->mMaxWidth = aMaxWidth;
  TTF_CloseFont(font);
  SDL_FreeSurface(msg);
  return textureData;
}

/**
 * @brief Set shader uniform by program id
 * @param aProgram Program that uniform is in
 * @param aName Name of uniform
 * @param aPropertyType Property type of uniform
 * @param aValue Value of uniform
 * @param aId Smapler id if applicable
 */
void ShaderLoader::SetShaderUniform(int aProgram, HashString const &aName, PropertyType const &aPropertyType, HashString const &aValue, HashString const &aId)
{
  switch(aPropertyType)
  {
    case PropertyType::INT1:
    {
      glUniform1i(glGetUniformLocation(aProgram, aName), aValue.ToInt());
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::INT2:
    {
      std::vector<int> intVector = aValue.ToIntVector();
      glUniform2i(glGetUniformLocation(aProgram, aName), intVector[0], intVector[1]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::INT3:
    {
      std::vector<int> intVector = aValue.ToIntVector();
      glUniform3i(glGetUniformLocation(aProgram, aName), intVector[0], intVector[1], intVector[2]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::INT4:
    {
      std::vector<int> intVector = aValue.ToIntVector();
      glUniform4i(glGetUniformLocation(aProgram, aName), intVector[0], intVector[1], intVector[2], intVector[3]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::FLOAT1:
    {
      glUniform1f(glGetUniformLocation(aProgram, aName), aValue.ToFloat());
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::FLOAT2:
    {
      std::vector<float> floatVector = aValue.ToFloatVector();
      glUniform2f(glGetUniformLocation(aProgram, aName), floatVector[0], floatVector[1]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::FLOAT3:
    {
      std::vector<float> floatVector = aValue.ToFloatVector();
      glUniform3f(glGetUniformLocation(aProgram, aName), floatVector[0], floatVector[1], floatVector[2]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::FLOAT4:
    {
      std::vector<float> floatVector = aValue.ToFloatVector();
      glUniform4f(glGetUniformLocation(aProgram, aName), floatVector[0], floatVector[1], floatVector[2], floatVector[3]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::SAMPLER2:
    {
      int id = aId.ToInt();
      glActiveTexture(GL_TEXTURE0 + id);
      GL_ERROR_CHECK();
      glBindTexture(GL_TEXTURE_2D, aValue.ToInt());
      GL_ERROR_CHECK();
      glUniform1i(glGetUniformLocation(aProgram, aName), id);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::INT1VECTOR:
    {
      std::vector<int> intVector = aValue.ToIntVector();
      glUniform1iv(glGetUniformLocation(aProgram, aName), intVector.size(), &intVector[0]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::INT2VECTOR:
    {
      std::vector<int> intVector = aValue.ToIntVector();
      glUniform2iv(glGetUniformLocation(aProgram, aName), intVector.size() / 2, &intVector[0]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::INT3VECTOR:
    {
      std::vector<int> intVector = aValue.ToIntVector();
      glUniform3iv(glGetUniformLocation(aProgram, aName), intVector.size() / 3, &intVector[0]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::INT4VECTOR:
    {
      std::vector<int> intVector = aValue.ToIntVector();
      glUniform4iv(glGetUniformLocation(aProgram, aName), intVector.size() / 4, &intVector[0]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::FLOAT1VECTOR:
    {
      std::vector<float> floatVector = aValue.ToFloatVector();
      glUniform1fv(glGetUniformLocation(aProgram, aName), floatVector.size(), &floatVector[0]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::FLOAT2VECTOR:
    {
      std::vector<float> floatVector = aValue.ToFloatVector();
      glUniform2fv(glGetUniformLocation(aProgram, aName), floatVector.size() / 2, &floatVector[0]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::FLOAT3VECTOR:
    {
      std::vector<float> floatVector = aValue.ToFloatVector();
      glUniform3fv(glGetUniformLocation(aProgram, aName), floatVector.size() / 3, &floatVector[0]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::FLOAT4VECTOR:
    {
      std::vector<float> floatVector = aValue.ToFloatVector();
      glUniform4fv(glGetUniformLocation(aProgram, aName), floatVector.size() / 4, &floatVector[0]);
      GL_ERROR_CHECK();
      break;
    }
    case PropertyType::SAMPLER2VECTOR:
    {
      std::vector<int> ids = aId.ToIntVector();
      std::vector<int> values = aValue.ToIntVector();
      
      if(ids.size() != values.size())
        assert(!"Sampler id and value sizes do not match. (PCShaderScreen.cpp)(SetShaderProperties)");
      
      for(unsigned i = 0; i < ids.size(); ++i)
      {
        glActiveTexture(GL_TEXTURE0 + ids[i]);
        GL_ERROR_CHECK();
        glBindTexture(GL_TEXTURE_2D, values[i]);
        GL_ERROR_CHECK();
      }
      glUniform1iv(glGetUniformLocation(aProgram, aName), ids.size(), &ids[0]);
      GL_ERROR_CHECK();
      break;
    }
    default:
    {
      assert(!"Invalid property type.");
      break;
    }
  }
}

/**
 * @brief Import texture into OpenGL.
 * @param aSurface Copy data from here.
 * @param aTextureFormat Format of image.
 * @param aMinFilter Min filter.
 * @param aMagFilter Mag filter.
 * @return Texture id.
 */
unsigned ShaderLoader::ImportTexture(SDL_Surface* aSurface, GLenum aTextureFormat, HashString const &aMinFilter, HashString const &aMagFilter)
{
  GLuint textureId = 0;
  GLTextureInfo info(aMinFilter, aMagFilter);
  GLint minFilter = info.mMinFilter;
  GLint magFilter = info.mMagFilter;
  GLint wrapS = info.mWrapS;
  GLint wrapT = info.mWrapT;

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
  glGenerateMipmap(GL_TEXTURE_2D);
  mSurfaces.push_back(aSurface);
  info.mTextureID = textureId;

  return textureId;
}

/**
 * @brief Clear all SDL_Surfaces.
 */
void ShaderLoader::Clear()
{
  for(std::vector<SDL_Surface*>::iterator it = mSurfaces.begin(); it != mSurfaces.end(); ++it)
  {
    SDL_FreeSurface(*it);
  }
  mSurfaces.clear();
}
