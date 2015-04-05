#include "PCSurface.h"
#include "Common.h"
#include "GraphicsManager.h"
#include "LUATypes.h"

#if !defined(_WIN32) && !defined(__APPLE__)
#include <SDL2/SDL_image.h>
#elif defined(__APPLE__)
#include <SDL2_image/SDL_image.h>
#else
#include "SDL_image.h"
#endif

PCSurface::PCSurface() : Surface()
{
}
PCSurface::PCSurface(GraphicsManager *aManager) : Surface(aManager), mSurface(NULL),
                                                  mWrapMode(GL_CLAMP_TO_EDGE), mFont(NULL)
{
}
PCSurface::~PCSurface()
{
  if(mFont)
    TTF_CloseFont(mFont);
  if(mSurface)
    SDL_FreeSurface(mSurface);
}

void PCSurface::LoadImage(std::string const &aName)
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
      DebugLogPrint("warning: width of image: %s is not a power of 2\n", aName.c_str());
    }

    if ((mSurface->h & (mSurface->h - 1)) != 0 )
    {
      DebugLogPrint("warning: height of image: %s is not a power of 2\n", aName.c_str());
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
      DebugLogPrint("warning: image %s is not truecolor..  this will probably break\n", aName.c_str());
    }

    glGenTextures(1, &mTextureID);

    glBindTexture(GL_TEXTURE_2D, mTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapMode);

    glTexImage2D(GL_TEXTURE_2D, 0, mNumberOfColors, mSurface->w, mSurface->h, 0,
              mTextureFormat, GL_UNSIGNED_BYTE, mSurface->pixels);
    
    GetManager()->AddTexturePairing(aName, TextureData(mTextureID, mSurface->w, mSurface->h));
  }
  else
  {
    DebugLogPrint("warning: file: %s not found or incompatible format, check this out\n", aName.c_str());
  }
}

// Returns the size of the Text texture
Vector3 PCSurface::LoadText(std::string const &aFont, std::string const &aText, Vector4 const &aForegroundColor, Vector4 const &aBackgroundColor, int aSize, int aMaxWidth)
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

  TextureData const& data = GetManager()->GetTextureData(aText);
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
    SDL_Surface *msg = TTF_RenderText_Blended_Wrapped(mFont, aText.c_str(), fgColor, aMaxWidth);
    assert(msg);

    mSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, msg->w, msg->h, 32, rmask, gmask, bmask, amask);
    SetTextureSize(Vector3(mSurface->w, mSurface->h, 0));
    SDL_BlitSurface(msg, NULL, mSurface, NULL);

    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mSurface->w, mSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, mSurface->pixels);

    GetManager()->AddTexturePairing(aText, TextureData(mTextureID, mSurface->w, mSurface->h));

    return Vector3(mSurface->w, mSurface->h, 0);
  }
}

unsigned PCSurface::GetIndexValue() const
{
  return mTextureID;
}

void PCSurface::SetWrapMode(WrapMode const &aWrapMode)
{
  switch(aWrapMode)
  {
  case REPEAT:
    mWrapMode = GL_REPEAT;
    break;
  case CLAMP:
    mWrapMode = GL_CLAMP_TO_EDGE;
    break;
  }
}

void PCSurface::Update()
{
  Surface::Update();
}
void PCSurface::SendMessage(Message const &aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}
void PCSurface::ReceiveMessage(Message const &aMessage)
{
}

void PCSurface::Serialize(Parser &aParser)
{
  std::string objectName = std::string("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  Root* object = aParser.Find(objectName);
  Surface::Serialize(aParser);
  object->Place("Surface", "TextureName", GetFileName());
}

void PCSurface::Deserialize(Parser &aParser)
{
  // Get file name if specified
  std::string fileName = "";
  if(aParser.Find("Surface", "TextureName"))
    fileName = aParser.Find("Surface", "TextureName")->GetValue().ToString();

  SetFileName(fileName);
  LoadImage(GetFileName());
  
  Surface::Deserialize(aParser);
}

void PCSurface::SerializeLUA()
{
  SLB::Class<PCSurface>("PCSurface")
    .inherits<Surface>();
}

unsigned PCSurface::GetTextureID() const
{
	return mTextureID;
}
