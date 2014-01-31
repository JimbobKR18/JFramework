//
//  TextureCoordinates.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/30/12.
//
//

#include "TextureCoordinates.h"

#define DT (1.0f/60.0f)
#define SETFRAMES() mXValues[0] = mCurFrame * mXGain; \
                    mXValues[1] = (mCurFrame + 1) * mXGain; \
                    mYValues[0] = mCurAnimation * mYGain; \
                    mYValues[1] = (mCurAnimation + 1) * mYGain

TextureCoordinates::TextureCoordinates()
{
  assert(0);
}
TextureCoordinates::TextureCoordinates(int const aNumAnimations, std::vector<int> const aNumFrames) : mCurFrame(0),
                                                                                                      mCurAnimation(0),
                                                                                                      mNumAnimations(aNumAnimations),
                                                                                                      mSpeed(DT),
                                                                                                      mCurTime(0),
                                                                                                      mCompleted(false)
{
  int maxFrames = 0;
  
  // Push back all frame data
  for(int i = 0; i < aNumAnimations; ++i)
  {
    mAnimations.insert(AnimationData(i, aNumFrames[i]));
    
    // Need to figure out each frame size
    if(aNumFrames[i] > maxFrames)
    {
      maxFrames = aNumFrames[i];
    }
  }
  
  // Figure out the gain per step of animation
  mXGain = 1.0f / (float)maxFrames;
  mYGain = 1.0f / (float)aNumAnimations;
  
  // Set all values to starting positions
  SETFRAMES();
}

TextureCoordinates::~TextureCoordinates()
{
  
}

void TextureCoordinates::Update(float aDT)
{
  if(!mAnimated)
    return;
  
  mCurTime += aDT;
  
  // If it's time to change a frame
  while(mCurTime >= mSpeed)
  {
    mCurTime -= mSpeed;
    
    // Increase the frame
    ++mCurFrame;
    
    // Make sure we're within bounds
    if(mCurFrame >= mAnimations.find(mCurAnimation)->second)
    {
      mCurFrame = 0;
      mCompleted = true;
    }
    
    // Set positions of coordinates
    SETFRAMES();
  }
}

float TextureCoordinates::GetXValue(int aValue) const
{
  return mXValues[aValue];
}

float TextureCoordinates::GetYValue(int aValue) const
{
  return mYValues[aValue];
}

int TextureCoordinates::GetCurrentAnimation() const
{
  return mCurAnimation;
}

int TextureCoordinates::GetNumberofAnimations() const
{
  return mAnimations.size();
}

int TextureCoordinates::GetAnimationFrameCounts(int const aAnimation) const
{
  return mAnimations.find(aAnimation)->second;
}

bool TextureCoordinates::GetCompleted() const
{
  return mCompleted;
}

bool TextureCoordinates::GetAnimated() const
{
  return mAnimated;
}

void TextureCoordinates::SetCurrentAnimation(int aAnimation)
{
  mCurAnimation = aAnimation;
  mCompleted = false;
}

void TextureCoordinates::SetCurrentFrame(int aFrame)
{
  mCurFrame = aFrame;
  
  // Set positions of coordinates
  SETFRAMES();
}

void TextureCoordinates::SetAnimated(bool aAnimated)
{
  mAnimated = aAnimated;
}

void TextureCoordinates::SetSpeed(float aSpeed)
{
  mSpeed = aSpeed;
}

void TextureCoordinates::Reset()
{
  mCurFrame = 0;
  mCurTime = 0;
  mCompleted = false;
}
