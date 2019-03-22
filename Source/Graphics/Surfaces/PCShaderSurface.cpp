#include "PCShaderSurface.h"
#include "Common.h"
#include "GraphicsManager.h"
#include "LUATypes.h"
#include "Constants.h"
#include "SystemProperties.h"
#include "ShaderLoader.h"
#include "Transform.h"

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

PCShaderSurface::PCShaderSurface(PCShaderSurface const &aPCShaderSurface) : Surface(aPCShaderSurface), mTextureID(aPCShaderSurface.mTextureID),
  mProgramID(aPCShaderSurface.mProgramID), mVertexShaderID(aPCShaderSurface.mVertexShaderID), mFragmentShaderID(aPCShaderSurface.mFragmentShaderID),
  mVertexShaderFileName(aPCShaderSurface.mVertexShaderFileName), mFragmentShaderFileName(aPCShaderSurface.mFragmentShaderFileName)
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
    textureData = ShaderLoader::LoadTexture(aName, GetMinFilter(), GetMagFilter());
    if(textureData)
    {
      SetTextureSize(Vector3(textureData->mWidth, textureData->mHeight, 0));
      GetManager()->AddTexturePairing(textureData->mTextureName, textureData);
      mTextureID = textureData->mTextureID;
    }
  }
}

/**
 * @brief Loads a text surface. Font is configured from deserialize.
 * @param aText Text to render.
 * @return 
 */
void PCShaderSurface::LoadText(HashString const &aText)
{
  HashString const textureDataHash = GetFontName() + aText + Common::IntToString(GetFontSize()) +
    GetMinFilter() + GetMagFilter();
  TextureData* data = GetManager()->GetTextureData(textureDataHash);
  Vector3 size;
  if(data->mTextureID != (unsigned)-1)
  {
    size = Vector3(data->mWidth / 2.0f, data->mHeight / 2.0f, 0);
    mTextureID = data->mTextureID;
    SetTextureSize(size);
  }
  else
  {
    TextureData* textureData = ShaderLoader::LoadText(GetFontName(), aText, GetMinFilter(), GetMagFilter(), 
      GetColor(), GetSecondaryColor(), GetFontSize(), GetMaxTextWidth());
    if(textureData)
    {
      size = Vector3(textureData->mWidth / 2.0f, textureData->mHeight / 2.0f, 0);
      SetTextureSize(size);
      GetManager()->AddTexturePairing(textureData->mTextureName, textureData);
      mTextureID = textureData->mTextureID;
    }
  }
  
  // Update texture to be the right size.
  Transform *transform = GetOwner()->GET<Transform>();
  transform->SetSize(size);
  SetOriginalSize(size);
  SetText(aText);
  
  if(GetTextRenderStyle() == DEFAULT_RENDER_STYLE)
  {
    std::vector<std::vector<float>> animationSpeed;
    animationSpeed.push_back(std::vector<float>());
    animationSpeed[0].push_back(GetManager()->GetOwningApp()->GetAppStep());
    
    std::vector<int> numFrames;
    numFrames.push_back(1);
    
    SetTextureCoordinateData(1, numFrames, animationSpeed);
    SetAnimated(false);
    SetAnimation(0);
  }
  else
  {
    std::vector<std::vector<float>> animationSpeeds;
    std::vector<int> numFrames;
    
    // Manually set the number of frames, or auto jump a character at a time.
    if(GetTextRenderStyle() == SMOOTH_RENDER_STYLE)
    {
      numFrames.push_back(GetTextureData()->GetAnimationFrameCounts(0));
    }
    else
    {
      numFrames.push_back(aText.Length());
    }

    std::vector<float> animationSpeed;
    for(int i = 0; i < numFrames[0]; ++i)
      animationSpeed.push_back(GetTextureData()->GetAnimationHolds(0)[0]);
    animationSpeeds.push_back(animationSpeed);

    CreateScrollEffect(ScrollType::HORIZONTAL, GetOriginalSize());
    SetTextureCoordinateData(1, numFrames, animationSpeeds);
    GetTextureData()->SetXGain(0, 0);
    SetAnimation(0, TextureCoordinateBehavior::RUN_ONCE);
    SetAnimated(true);
    transform->GetSize().x = 0;
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
  Surface::ReceiveMessage(aMessage);
}

/**
 * @brief Serialize to file.
 * @param aNode ParserNode to write into.
 */
void PCShaderSurface::Serialize(ParserNode *aNode)
{
  HashString const SURFACE = "Surface";
  Surface::Serialize(aNode);
  ParserNode* surface = aNode->Find(SURFACE);
  surface->Place("TextureName", GetFileName());
  surface->Place("VertexShader", mVertexShaderFileName);
  surface->Place("FragmentShader", mFragmentShaderFileName);
}

/**
 * @brief Read from file.
 * @param aNode ParserNode to read from.
 */
void PCShaderSurface::Deserialize(ParserNode *aNode)
{
  Surface::Deserialize(aNode);
  HashString vertexShader = (GetVertexShaderFilename() == "") ? SystemProperties::GetDefaultVertexShaderName() : GetVertexShaderFilename();
  HashString fragmentShader = (GetFragmentShaderFilename() == "") ? SystemProperties::GetDefaultFragmentShaderName() : GetFragmentShaderFilename();
  
  if(aNode->Find("VertexShader"))
    vertexShader = aNode->Find("VertexShader")->GetValue().ToString();
  if(aNode->Find("FragmentShader"))
    fragmentShader = aNode->Find("FragmentShader")->GetValue().ToString();

  if(aNode->GetName() == "Surface")
  {
    HashString fileName = GetFileName();
    if(aNode->Find("TextureName"))
      fileName = aNode->Find("TextureName")->GetValue().ToString();
      
    SetFileName(fileName);
    LoadImage(GetFileName());
  }
  else if(aNode->GetName() == "Text")
  {
    TextRenderStyle renderStyle = TextRenderStyle::DEFAULT_RENDER_STYLE;
    if(aNode->Find("RenderStyle"))
    {
      if(aNode->Find("RenderStyle")->GetValue() == "SMOOTH")
        renderStyle = TextRenderStyle::SMOOTH_RENDER_STYLE;
      else if(aNode->Find("RenderStyle")->GetValue() == "CHARACTER_BY_CHARACTER")
        renderStyle = TextRenderStyle::CHARACTER_BY_CHARACTER_STYLE;
      else
      {
        DebugLogPrint("Incorrect render style %s used for text rendering.", aNode->Find("RenderStyle")->GetValue().ToCharArray());
        assert(!"Incorrect render style used for text rendering.");
      }
    }
    SetTextRenderStyle(renderStyle);
    if(aNode->Find("Contents"))
      LoadText(aNode->Find("Contents")->GetValue());
  }
  else
  {
    assert(!"Node has wrong name for deserialization");
  }
  
  LoadShaders(vertexShader, fragmentShader);
}

/**
 * @brief Copy surface
 * @param aNewOwner The new owner
 * @return Copied surface
 */
Component* PCShaderSurface::Clone(GameObject *aNewOwner) const
{
  return new PCShaderSurface(*this);
}

/**
 * @brief LUA functionality.
 */
void PCShaderSurface::SerializeLUA()
{
  SLB::Class<PCShaderSurface>("PCShaderSurface")
    .inherits<Surface>();
}
