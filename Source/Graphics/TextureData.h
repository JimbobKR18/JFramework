/*
 * TextureData.h
 *
 *  Created on: Feb 23, 2014
 *      Author: jimbob
 */

#ifndef TEXTUREDATA_H_
#define TEXTUREDATA_H_

#include "Common.h"
#include "MathExt.h"

struct TextureData
{
  // Common
  HashString mTextureName;
  unsigned mTextureID;
  unsigned mWidth;
  unsigned mHeight;
  
  // Text
  HashString mFont; 
  HashString mText;
  Vector4 mForegroundColor;
  Vector4 mBackgroundColor; 
  int mSize;
  int mMaxWidth;

  TextureData() : mTextureName(), mTextureID(-1), mWidth(0), mHeight(0), mFont(), mText(), mForegroundColor(),
                  mBackgroundColor(), mSize(0), mMaxWidth(0) {}
  TextureData(TextureData const &aRhs) : mTextureName(aRhs.mTextureName), mTextureID(aRhs.mTextureID), 
  mWidth(aRhs.mWidth), mHeight(aRhs.mHeight), mFont(aRhs.mFont), mText(aRhs.mText), mForegroundColor(aRhs.mForegroundColor),
  mBackgroundColor(aRhs.mBackgroundColor), mSize(aRhs.mSize), mMaxWidth(aRhs.mMaxWidth)
  {
  }
  TextureData(HashString const &aTextureName, unsigned aTextureID, unsigned aWidth, unsigned aHeight,
              HashString const &aFont, HashString const &aText, Vector4 const &aForegroundColor, 
              Vector4 const &aBackgroundColor, int aSize, int aMaxWidth) :
    mTextureName(aTextureName), mTextureID(aTextureID), mWidth(aWidth), mHeight(aHeight), mFont(aFont), mText(aText),
    mForegroundColor(aForegroundColor), mBackgroundColor(aBackgroundColor), mSize(aSize), mMaxWidth(aMaxWidth)
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
    mFont = aRhs.mFont; 
    mText = aRhs.mText;
    mForegroundColor = aRhs.mForegroundColor;
    mBackgroundColor = aRhs.mBackgroundColor;
    mSize = aRhs.mSize; 
    mMaxWidth = aRhs.mMaxWidth;
  }
};

#endif /* TEXTUREDATA_H_ */
