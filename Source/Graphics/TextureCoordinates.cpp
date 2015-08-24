//
//  TextureCoordinates.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/30/12.
//
//

#include "TextureCoordinates.h"

#define SETFRAMES() mXValues[0] = mCurFrame * mXGain[0] + (0.5f / (float)mXSize); \
                    mXValues[1] = (mCurFrame + 1) * mXGain[1] - (0.5f / (float)mXSize); \
                    mYValues[0] = mCurAnimation * mYGain[0] + (0.5f / (float)mYSize); \
                    mYValues[1] = (mCurAnimation + 1) * mYGain[1] - (0.5f / (float)mYSize)

TextureCoordinates::TextureCoordinates()
{
  assert(0);
}
TextureCoordinates::TextureCoordinates(int const aXSize,
                                       int const aYSize, 
                                       int const aNumAnimations,
                                       std::vector<int> const aNumFrames,
                                       float aAnimationSpeed) : mCurFrame(0),
                                                                mCurAnimation(0),
                                                                mTotalFrames(0),
                                                                mXSize(aXSize),
                                                                mYSize(aYSize),
                                                                mSpeed(aAnimationSpeed),
                                                                mCurTime(0),
                                                                mAnimated(false),
                                                                mCompleted(false),
                                                                mRunOnce(false)
{
  int maxFrames = 0;
  
  // Push back all frame data
  for(int i = 0; i < aNumAnimations; ++i)
  {
    mTotalFrames += aNumFrames[i];
    mAnimations.insert(AnimationData(i, aNumFrames[i]));
    
    // Need to figure out each frame size
    if(aNumFrames[i] > maxFrames)
    {
      maxFrames = aNumFrames[i];
    }
  }
  
  // Figure out the gain per step of animation
  mXGain[0] = mXGain[1] = 1.0f / (float)maxFrames;
  mYGain[0] = mYGain[1] = 1.0f / (float)aNumAnimations;
  
  // Set all values to starting positions
  SETFRAMES();
}

TextureCoordinates::~TextureCoordinates()
{
  
}

/**
 * @brief Basic update loop, updates texture coordinates based on DT
 * @param aDT
 */
void TextureCoordinates::Update(float aDT)
{
  if(!mAnimated)
  {
    // If it's not animated, the animation is complete.
    mCompleted = true;
    return;
  }
  
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
      mCompleted = true;
      // If only run once, do not reset the frames.
      if(!mRunOnce)
      {
        mCurFrame = 0;
      }
      else
      {
        return;
      }
    }
    
    // Set positions of coordinates
    SETFRAMES();
  }
}

/**
 * @brief Get current x coordinate.
 * @param aIndex 0 for left, 1 for right.
 * @return 
 */
float TextureCoordinates::GetXValue(int const aIndex) const
{
  return mXValues[aIndex];
}

/**
 * @brief Get current y coordinate.
 * @param aIndex 0 for top, 1 for bottom.
 * @return 
 */
float TextureCoordinates::GetYValue(int const aIndex) const
{
  return mYValues[aIndex];
}

/**
 * @brief Get animation speed.
 * @return 
 */
float TextureCoordinates::GetAnimationSpeed() const
{
  return mSpeed;
}

/**
 * @brief Get current animation index.
 * @return 
 */
int TextureCoordinates::GetCurrentAnimation() const
{
  return mCurAnimation;
}

/**
 * @brief Get the total number of animations.
 * @return 
 */
int TextureCoordinates::GetNumberOfAnimations() const
{
  return mAnimations.size();
}

/**
 * @brief Get the total number of frames across all animations.
 * @return 
 */
int TextureCoordinates::GetTotalFrames() const
{
  return mTotalFrames;
}

/**
 * @brief Get number of frames for an animation
 * @param aAnimation The animation index.
 * @return 
 */
int TextureCoordinates::GetAnimationFrameCounts(int const aAnimation) const
{
  return mAnimations.find(aAnimation)->second;
}

/**
 * @brief Is the current animation finished cycling through?
 * @return 
 */
bool TextureCoordinates::GetCompleted() const
{
  return mCompleted;
}

/**
 * @brief Are we animated at all?
 * @return 
 */
bool TextureCoordinates::GetAnimated() const
{
  return mAnimated;
}

/**
 * @brief Sets the current animation, starts from beginning.
 * @param aAnimation
 */
void TextureCoordinates::SetCurrentAnimation(int const aAnimation)
{
  // So as to not reset the animation, we have an early out.
  if(mCurAnimation == aAnimation)
  {
    return;
  }
  // Assert in case of terribleness.
  else if(aAnimation >= mAnimations.size())
  {
    assert(!"SetCurrentAnimation: aAnimation is larger than the number of animations.");
  }
  
  mCurAnimation = aAnimation;
  mCurFrame = 0;
  mCompleted = false;
  
  // Set positions of coordinates
  SETFRAMES();
}

/**
 * @brief Set current frame in animation
 * @param aFrame
 */
void TextureCoordinates::SetCurrentFrame(int const aFrame)
{
  if(mCurFrame > mAnimations.find(mCurAnimation)->second)
  {
    assert(!"SetCurrentFrame: aFrame is larger than number of frames in animation");
  }

  mCurFrame = aFrame;
  
  // Set positions of coordinates
  SETFRAMES();
}

/**
 * @brief Across all frames, what index do you want to be in?
 * @param aFrameID
 */
void TextureCoordinates::SetFrameByID(int const aFrameID)
{
  // Set frame, if above frames for current animation,
  // move onto next animation, so on and so on.
  if(aFrameID > mTotalFrames)
  {
    assert(!"SetFrameByID: aFrameID exceeds total frames in animation.");
  }

  mCurAnimation = 0;
  mCurFrame = aFrameID;

  while(mCurFrame >= mAnimations.find(mCurAnimation)->second)
  {
    mCurFrame -= mAnimations.find(mCurAnimation)->second;
    ++mCurAnimation;
  }

  // Set positions of coordinates
  SETFRAMES();
}

/**
 * @brief Set whether or not to animate this object.
 * @param aAnimated
 */
void TextureCoordinates::SetAnimated(bool const aAnimated)
{
  mAnimated = aAnimated;
}

/**
 * @brief Set whether or not to run the current animation only once.
 * @param aRunOnce
 */
void TextureCoordinates::SetRunOnce(bool const aRunOnce)
{
  mRunOnce = aRunOnce;
}

/**
 * @brief Set time before next frame (in seconds).
 * @param aSpeed
 */
void TextureCoordinates::SetSpeed(float const aSpeed)
{
  mSpeed = aSpeed;
}

/**
 * @brief Set pace in which the x coordinate should gain per frame.
 * @param aIndex 0 for left, 1 for right.
 * @param aX The gain.
 */
void TextureCoordinates::SetXGain(int const aIndex, float const aXGain)
{
  mXGain[aIndex] = aXGain;
}

/**
 * @brief Set pace in which the y coordinate should gain per animation.
 * @param aIndex 0 for top, 1 for bottom.
 * @param aY The gain.
 */
void TextureCoordinates::SetYGain(int const aIndex, float const aYGain)
{
  mYGain[aIndex] = aYGain;
}

/**
 * @brief Finish current animation.
 */
void TextureCoordinates::Finish()
{
  // Set to last frame.
  mCurFrame = mAnimations.find(mCurAnimation)->second - 1;
  // Set positions of coordinates
  SETFRAMES();
}

/**
 * @brief Start animation at the beginning.
 */
void TextureCoordinates::Reset()
{
  mCurFrame = 0;
  mCurTime = 0;
  mCompleted = false;
}
