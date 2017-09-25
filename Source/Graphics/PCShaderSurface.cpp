#include "PCShaderSurface.h"
#include "Common.h"
#include "GraphicsManager.h"
#include "LUATypes.h"
#include "Constants.h"
#include "SystemProperties.h"
#include "ShaderLoader.h"

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

PCShaderSurface::PCShaderSurface() : Surface(), mTextureID(-1), mProgramID(0), mVertexShaderID(0), mFragmentShaderID(0),
                                     mVertexShaderFileName(), mFragmentShaderFileName()
{
  assert(!"Do not use");
}
PCShaderSurface::PCShaderSurface(GraphicsManager *aManager) : Surface(aManager), mTextureID(-1), mProgramID(0), mVertexShaderID(0),
                                                              mFragmentShaderID(0), mVertexShaderFileName(), mFragmentShaderFileName()
{
}

PCShaderSurface::~PCShaderSurface()
{
}

/**
 * @brief Load an image by file name.
 * @param aName Name of the file.
 */
void PCShaderSurface::LoadImage(HashString const &aName)
{
  /* If the file was already loaded,
     let's avoid assigning a new id. */
  TextureData* textureData = GetManager()->GetTextureData(aName);
  if(textureData->mTextureID != (unsigned)-1)
  {
    mTextureID = textureData->mTextureID;
    SetTextureSize(Vector3(textureData->mWidth, textureData->mHeight, 0));
  }
  // else we load the image from file
  else
  {
    TextureData *textureData = ShaderLoader::LoadTexture(aName);
    if(textureData)
    {
      SetTextureSize(Vector3(textureData->mWidth, textureData->mHeight, 0));
      GetManager()->AddTexturePairing(textureData->mTextureName, textureData);
      mTextureID = textureData->mTextureID;
    }
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
  HashString const textureDataHash = aFont + aText + Common::IntToString(aSize);
  TextureData* data = GetManager()->GetTextureData(textureDataHash);
  if(data->mTextureID != (unsigned)-1)
  {
    Vector3 size = Vector3(data->mWidth, data->mHeight, 0);
    mTextureID = data->mTextureID;
    SetTextureSize(size);
    return size;
  }
  else
  {
    TextureData* textureData = ShaderLoader::LoadText(aFont, aText, aForegroundColor, aBackgroundColor, aSize, aMaxWidth);
    if(textureData)
    {
      SetTextureSize(Vector3(textureData->mWidth, textureData->mHeight, 0));
      GetManager()->AddTexturePairing(textureData->mTextureName, textureData);
      mTextureID = textureData->mTextureID;
      return Vector3(textureData->mWidth, textureData->mHeight, 0);
    }
    else
    {
      return Vector3(0, 0, 0);
    }
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
    mProgramID = GetManager()->GetShaderData(shaderKey)->mProgramID;
    return;
  }
  
  ShaderData *shaderData = ShaderLoader::LoadShaders(aVertexShaderFilename, aFragmentShaderFilename);
  mProgramID = shaderData->mProgramID;
  mVertexShaderID = shaderData->mVertexShaderID;
  mFragmentShaderID = shaderData->mFragmentShaderID;
  GetManager()->AddShaderPairing(shaderKey, shaderData);
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
 * @brief Get GL specified vertex shader ID.
 * @return Vertex shader ID.
 */
unsigned PCShaderSurface::GetVertexShaderID() const
{
  return mVertexShaderID;
}

/**
 * @brief Get GL specified fragment shader ID.
 * @return Fragment shader ID.
 */
unsigned PCShaderSurface::GetFragmentShaderID() const
{
  return mFragmentShaderID;
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
 * @brief Set texture ID.
 * @param aTextureID Texture ID.
 */
void PCShaderSurface::SetTextureID(unsigned const aTextureID)
{
  mTextureID = aTextureID;
}

/**
 * @brief Set program ID.
 * @param aProgramID Program ID.
 */
void PCShaderSurface::SetProgramID(unsigned const aProgramID)
{
  mProgramID = aProgramID;
}

/**
 * @brief Set vertex shader ID.
 * @param aVertexShaderID Vertex shader ID.
 */
void PCShaderSurface::SetVertexShaderID(unsigned const aVertexShaderID)
{
  mVertexShaderID = aVertexShaderID;
}

/**
 * @brief Set fragment shader ID.
 * @param aFragmentShaderID Fragment shader ID.
 */
void PCShaderSurface::SetFragmentShaderID(unsigned const aFragmentShaderID)
{
  mFragmentShaderID = aFragmentShaderID;
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
  HashString vertexShader = SystemProperties::GetDefaultVertexShaderName();
  HashString fragmentShader = SystemProperties::GetDefaultFragmentShaderName();
  
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
