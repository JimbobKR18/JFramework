/*
 * TextureData.h
 *
 *  Created on: Feb 23, 2014
 *      Author: jimbob
 */

#ifndef TEXTUREDATA_H_
#define TEXTUREDATA_H_

#include "Common.h"

struct TextureData
{
  HashString mTextureName;
  unsigned mTextureID;
  unsigned mWidth;
  unsigned mHeight;

  TextureData() : mTextureName(), mTextureID(-1), mWidth(0), mHeight(0) {}
  TextureData(TextureData const &aRhs) : mTextureName(aRhs.mTextureName), mTextureID(aRhs.mTextureID), 
  mWidth(aRhs.mWidth), mHeight(aRhs.mHeight)
  {
  }
  TextureData(HashString const &aTextureName, unsigned aTextureID, unsigned aWidth, unsigned aHeight) :
    mTextureName(aTextureName), mTextureID(aTextureID), mWidth(aWidth), mHeight(aHeight)
  {
  }
  virtual ~TextureData()
  {
  }
  void operator=(TextureData const &aRhs)
  {
    mTextureName = aRhs.mTextureName;
    mTextureID = aRhs.mTextureID;
    mWidth = aRhs.mWidth;
    mHeight = aRhs.mHeight;
  }
};

#endif /* TEXTUREDATA_H_ */
