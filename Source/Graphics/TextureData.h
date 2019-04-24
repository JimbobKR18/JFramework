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
#include "SystemProperties.h"

struct TextureData
{
  // Common
  HashString mTextureName;
  unsigned mTextureID;
  Vector2 mSize;
  HashString mMinFilter;
  HashString mMagFilter;
  
  // Text
  HashString mFont; 
  HashString mText;
  Vector4 mForegroundColor;
  Vector4 mBackgroundColor; 
  int mFontSize;
  int mMaxWidth;

  TextureData() : mTextureName(), mTextureID(-1), mSize(),
                  mMinFilter(SystemProperties::GetMinFilter()), mMagFilter(SystemProperties::GetMagFilter()),
                  mFont(), mText(), mForegroundColor(),
                  mBackgroundColor(), mFontSize(0), mMaxWidth(0) {}
  TextureData(TextureData const &aRhs) : mTextureName(aRhs.mTextureName), mTextureID(aRhs.mTextureID), 
  mSize(aRhs.mSize), mMinFilter(aRhs.mMinFilter), mMagFilter(aRhs.mMagFilter), 
  mFont(aRhs.mFont), mText(aRhs.mText), mForegroundColor(aRhs.mForegroundColor), mBackgroundColor(aRhs.mBackgroundColor), 
  mFontSize(aRhs.mFontSize), mMaxWidth(aRhs.mMaxWidth)
  {
  }
  TextureData(HashString const &aTextureName, unsigned aTextureID, Vector2 const &aSize,
              HashString const &aMinFilter, HashString const &aMagFilter,
              HashString const &aFont, HashString const &aText, Vector4 const &aForegroundColor, 
              Vector4 const &aBackgroundColor, int aFontSize, int aMaxWidth) :
    mTextureName(aTextureName), mTextureID(aTextureID), mSize(aSize), 
    mMinFilter(aMinFilter), mMagFilter(aMagFilter), mFont(aFont), mText(aText),
    mForegroundColor(aForegroundColor), mBackgroundColor(aBackgroundColor), mFontSize(aFontSize), mMaxWidth(aMaxWidth)
  {
  }
  virtual ~TextureData()
  {
  }
  void operator=(TextureData const &aRhs)
  {
    mTextureName = aRhs.mTextureName;
    mTextureID = aRhs.mTextureID;
    mSize = aRhs.mSize;
    mFont = aRhs.mFont; 
    mText = aRhs.mText;
    mForegroundColor = aRhs.mForegroundColor;
    mBackgroundColor = aRhs.mBackgroundColor;
    mFontSize = aRhs.mFontSize; 
    mMaxWidth = aRhs.mMaxWidth;
    mMinFilter = aRhs.mMinFilter;
    mMagFilter = aRhs.mMagFilter;
  }
};

#endif /* TEXTUREDATA_H_ */
