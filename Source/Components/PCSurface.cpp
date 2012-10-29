#include "PCSurface.h"
#include "Common.h"

PCSurface::PCSurface() : Surface()
{

}
PCSurface::~PCSurface()
{

}

void PCSurface::LoadImage(std::string const &aName)
{
	if((mSurface = SDL_LoadBMP(RelativePath(aName).c_str())))
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
				mTextureFormat = GL_BGRA;
		}
		else if (mNumberOfColors == 3)
		{
			if (mSurface->format->Rmask == 0x000000ff)
				mTextureFormat = GL_RGB;
			else
				mTextureFormat = GL_BGR;
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
	}
	else
	{
		printf("warning: file not found or incompatible format, check this out\n");
	}
}

void PCSurface::Update()
{

}
void PCSurface::SendMessage(Message const &aMessage)
{

}
void PCSurface::ReceiveMessage(Message const &aMessage)
{

}
