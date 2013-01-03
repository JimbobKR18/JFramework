#include "PCSurface.h"
#include "Common.h"
#include "GraphicsManager.h"

#if !defined(_WIN32) && !defined(__APPLE__)
#include <SDL/SDL_image.h>
#elif defined(__APPLE__)
#include <SDL_image/SDL_image.h>
#else
#include "SDL_image.h"
#endif

PCSurface::PCSurface() : Surface()
{
}
PCSurface::PCSurface(GraphicsManager *aManager) : Surface(aManager)
{
}
PCSurface::~PCSurface()
{
}

void PCSurface::LoadImage(std::string const &aName)
{
  /* If the file was already loaded,
     let's avoid assigning a new id. */
  if(mManager->GetTextureID(aName) != (unsigned)-1)
  {
    mTextureID = mManager->GetTextureID(aName);
  }
  // else we load the image from file
	else if((mSurface = IMG_Load(RelativePath(aName).c_str())))
	{
		if ((mSurface->w & (mSurface->w - 1)) != 0 )
		{
			printf("warning: image.bmp's width is not a power of 2\n");
		}

		if ((mSurface->h & (mSurface->h - 1)) != 0 )
		{
			printf("warning: image.bmp's height is not a power of 2\n");
		}

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
			printf("warning: the image is not truecolor..  this will probably break\n");
		}

		glGenTextures(1, &mTextureID);

		glBindTexture(GL_TEXTURE_2D, mTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, mNumberOfColors, mSurface->w, mSurface->h, 0,
						  mTextureFormat, GL_UNSIGNED_BYTE, mSurface->pixels);
    
    mManager->AddTexturePairing(aName, mTextureID);
	}
	else
	{
		printf("warning: file not found or incompatible format, check this out\n");
	}
}

unsigned PCSurface::GetIndexValue()
{
  return mTextureID;
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

unsigned PCSurface::GetTextureID() const
{
	return mTextureID;
}
