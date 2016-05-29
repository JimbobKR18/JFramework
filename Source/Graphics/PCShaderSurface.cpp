#include "PCShaderSurface.h"
#include "Common.h"
#include "GraphicsManager.h"
#include "LUATypes.h"
#include "Constants.h"

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

int const PCShaderSurface::sUID = Common::StringHashFunction("Surface");

PCShaderSurface::PCShaderSurface() : Surface(), mSurface(nullptr), mTextureFormat(), mNumberOfColors(0), mFont(nullptr),
                                     mVertexShaderFileName(), mFragmentShaderFileName()
{
}
PCShaderSurface::PCShaderSurface(GraphicsManager *aManager) : Surface(aManager), mSurface(nullptr), mTextureFormat(), 
                                                              mNumberOfColors(0), mFont(nullptr),
                                                              mVertexShaderFileName(), mFragmentShaderFileName()
{
}
PCShaderSurface::~PCShaderSurface()
{
  if(mFont)
  {
    TTF_CloseFont(mFont);
    mFont = nullptr;
  }
  if(mSurface)
  {
    SDL_FreeSurface(mSurface);
    mSurface = nullptr;
  }
}

/**
 * @brief Load an image by file name.
 * @param aName Name of the file.
 */
void PCShaderSurface::LoadImage(HashString const &aName)
{
  /* If the file was already loaded,
     let's avoid assigning a new id. */
  TextureData const& textureData = GetManager()->GetTextureData(aName);
  if(textureData.mTextureID != (unsigned)-1)
  {
    mTextureID = textureData.mTextureID;
    SetTextureSize(Vector3(textureData.mWidth, textureData.mHeight, 0));
  }
  // else we load the image from file
  else if((mSurface = IMG_Load(Common::RelativePath("Art", aName).c_str())))
  {
    if ((mSurface->w & (mSurface->w - 1)) != 0 )
    {
      DebugLogPrint("warning: width of image: %s is not a power of 2\n", aName.ToCharArray());
    }

    if ((mSurface->h & (mSurface->h - 1)) != 0 )
    {
      DebugLogPrint("warning: height of image: %s is not a power of 2\n", aName.ToCharArray());
    }
    
    SetTextureSize(Vector3(mSurface->w, mSurface->h, 0));
    mNumberOfColors = mSurface->format->BytesPerPixel;
    if (mNumberOfColors == 4)
    {
      if (mSurface->format->Rmask == 0x000000ff)
        mTextureFormat = GL_RGBA;
      else
#ifndef _WIN32
        mTextureFormat = GL_BGRA;
#else
        mTextureFormat = GL_RGBA;
#endif
    }
    else if (mNumberOfColors == 3)
    {
      if (mSurface->format->Rmask == 0x000000ff)
        mTextureFormat = GL_RGB;
      else
#ifndef _WIN32
        mTextureFormat = GL_BGR;
#else
        mTextureFormat = GL_RGB;
#endif
    }
    else
    {
      DebugLogPrint("warning: image %s is not truecolor...  this will probably break\n", aName.ToCharArray());
      DebugLogPrint("warning: bytes per pixel for image %s: %d\n", aName.ToCharArray(), mNumberOfColors);
    }

    AddTexturePairing(aName);
  }
  else
  {
    DebugLogPrint("warning: file: %s not found or incompatible format, check this out\n", aName.ToCharArray());
  }
}

/**
 * @brief Loads a text surface in font.
 * @param aFont Font to use.
 * @param aText Text to render.
 * @param aForegroundColor Color of the text.
 * @param aBackgroundColor Color of the background.
 * @param aSize Size of font.
 * @param aMaxWidth Max width of a single line (in pixels).
 * @return 
 */
Vector3 PCShaderSurface::LoadText(HashString const &aFont, HashString const &aText, Vector4 const &aForegroundColor, Vector4 const &aBackgroundColor, int aSize, int aMaxWidth)
{
  // Endianness is important here
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

  HashString const textureDataHash = aFont + aText + Common::IntToString(aSize);
  TextureData const& data = GetManager()->GetTextureData(textureDataHash);
  if(data.mTextureID != (unsigned)-1)
  {
    Vector3 size = Vector3(data.mWidth, data.mHeight, 0);
    mTextureID = data.mTextureID;
    SetTextureSize(size);
    return size;
  }
  else
  {
    if(!TTF_WasInit())
      TTF_Init();
    mFont = TTF_OpenFont(Common::RelativePath("Fonts", aFont).c_str(), aSize);
    if(!mFont)
    {
      mFont = NULL;
      DebugLogPrint("warning: file not found or incompatible format, check this out\n");
      DebugLogPrint("%s", TTF_GetError());
      return Vector3(0, 0, 0);
    }

    // Create text texture
    SDL_Color fgColor = {(Uint8)aForegroundColor.x, (Uint8)aForegroundColor.y, (Uint8)aForegroundColor.z, (Uint8)aForegroundColor.w};
    //SDL_Color bgColor = {(Uint8)aBackgroundColor.x, (Uint8)aBackgroundColor.y, (Uint8)aBackgroundColor.z, (Uint8)aBackgroundColor.w};
    SDL_Surface *msg = TTF_RenderText_Blended_Wrapped(mFont, aText.ToCharArray(), fgColor, aMaxWidth);
    if(!msg)
    {
      DebugLogPrint("TTF_RenderText failed: %s", TTF_GetError());
      assert(msg);
    }

    mTextureFormat = GL_RGBA;
    mSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, msg->w, msg->h, 32, rmask, gmask, bmask, amask);
    SetTextureSize(Vector3(mSurface->w, mSurface->h, 0));
    SDL_BlitSurface(msg, NULL, mSurface, NULL);

    AddTexturePairing(textureDataHash);

    return Vector3(mSurface->w, mSurface->h, 0);
  }
}

/**
 * @brief Load shaders.
 * @param aVertexShaderFilename Name of vertex shader.
 * @param aFragmentShaderFilename Name of fragment shader.
 */
void PCShaderSurface::LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename)
{
  mVertexShaderFileName = aVertexShaderFilename;
  mFragmentShaderFileName = aFragmentShaderFilename;
  
  HashString shaderKey = aVertexShaderFilename + aFragmentShaderFilename;
  
  if(GetManager()->ShaderDataExists(shaderKey))
  {
    mProgramID = GetManager()->GetShaderData(shaderKey).mProgramID;
    return;
  }
  
  std::ifstream vertexFile(Common::RelativePath("Shaders", aVertexShaderFilename.ToCharArray()).c_str());
  std::ifstream fragmentFile(Common::RelativePath("Shaders", aFragmentShaderFilename.ToCharArray()).c_str());
  if(vertexFile.is_open() && fragmentFile.is_open())
  {
    GLenum program = glCreateProgram();
    GLenum vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLenum fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
    
    GetManager()->AddShaderPairing(shaderKey, ShaderData(program, vertexShader, fragmentShader, vertexContents, fragmentContents));
    
    mProgramID = program;
  }
  else
  {
    DebugLogPrint("No shader with name %s or %s found\n", aVertexShaderFilename.ToCharArray(), aFragmentShaderFilename.ToCharArray());
    assert(!"Shader file not found.");
  }
}

/**
 * @brief Get GL specified texture value.
 * @return Texture ID.
 */
unsigned PCShaderSurface::GetTextureID() const
{
  return mTextureID;
}

/**
 * @brief Get GL specified program value.
 * @return Program ID.
 */
unsigned PCShaderSurface::GetProgramID() const
{
  return mProgramID;
}

/**
 * @brief Get vertex shader file name.
 * @return File name.
 */
HashString const& PCShaderSurface::GetVertexShaderFilename() const
{
  return mVertexShaderFileName;
}

/**
 * @brief Get fragment shader file name.
 * @return File name.
 */
HashString const& PCShaderSurface::GetFragmentShaderFilename() const
{
  return mFragmentShaderFileName;
}

/**
 * @brief Simple update loop.
 */
void PCShaderSurface::Update()
{
  Surface::Update();
}

/**
 * @brief Send message to owner.
 * @param aMessage Message to send.
 */
void PCShaderSurface::SendMessage(Message const &aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

/**
 * @brief Does nothing.
 * @param aMessage Message to receive.
 */
void PCShaderSurface::ReceiveMessage(Message const &aMessage)
{
}

/**
 * @brief Serialize to file.
 * @param aParser File to write into.
 */
void PCShaderSurface::Serialize(Parser &aParser)
{
  HashString const objectName = HashString("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  HashString const SURFACE = "Surface";
  Root* object = aParser.Find(objectName);
  Surface::Serialize(aParser);
  Root* surface = object->Find(SURFACE);
  surface->Place(SURFACE, "TextureName", GetFileName());
  surface->Place(SURFACE, "VertexShader", mVertexShaderFileName);
  surface->Place(SURFACE, "FragmentShader", mFragmentShaderFileName);
}

/**
 * @brief Read from file.
 * @param aParser File to read from.
 */
void PCShaderSurface::Deserialize(Parser &aParser)
{
  HashString fileName = "";
  HashString vertexShader = Constants::GetString("DefaultVertexShaderFileName");
  HashString fragmentShader = Constants::GetString("DefaultFragmentShaderFileName");
  
  if(aParser.Find("Surface", "TextureName"))
    fileName = aParser.Find("Surface", "TextureName")->GetValue().ToString();
  if(aParser.Find("Surface", "VertexShader"))
    vertexShader = aParser.Find("Surface", "VertexShader")->GetValue().ToString();
  if(aParser.Find("Surface", "FragmentShader"))
    fragmentShader = aParser.Find("Surface", "FragmentShader")->GetValue().ToString();

  SetFileName(fileName);
  LoadImage(GetFileName());
  LoadShaders(vertexShader, fragmentShader);
  
  Surface::Deserialize(aParser);
}

/**
 * @brief LUA functionality.
 */
void PCShaderSurface::SerializeLUA()
{
  SLB::Class<PCShaderSurface>("PCShaderSurface")
    .inherits<Surface>();
}

/**
 * @brief Helper to set basic texture params via config file.
 * @param aName Name of object in texture pairing.
 */
void PCShaderSurface::AddTexturePairing(HashString const &aName)
{
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

  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mSurface->w, mSurface->h, 0, mTextureFormat, GL_UNSIGNED_BYTE, mSurface->pixels);

  GetManager()->AddTexturePairing(aName, TextureData(mTextureID, mSurface->w, mSurface->h));
}