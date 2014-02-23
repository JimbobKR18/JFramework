/*
 * TextureData.h
 *
 *  Created on: Feb 23, 2014
 *      Author: jimbob
 */

#ifndef TEXTUREDATA_H_
#define TEXTUREDATA_H_

struct TextureData
{
  unsigned mTextureID;
  unsigned mWidth;
  unsigned mHeight;

  TextureData() : mTextureID(0), mWidth(0), mHeight(0) {}
  TextureData(unsigned aTextureID, unsigned aWidth, unsigned aHeight) :
    mTextureID(aTextureID), mWidth(aWidth), mHeight(aHeight) {}
  void operator=(TextureData const &aRhs)
  {
    mTextureID = aRhs.mTextureID;
    mWidth = aRhs.mWidth;
    mHeight = aRhs.mHeight;
  }
};

#endif /* TEXTUREDATA_H_ */
