//
//  TextureCoordinates.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/30/12.
//
//

#include "TextureCoordinates.h"

#define SETFRAMES() float xBias = (0.5f / (float)mXSize); \
                    float yBias = (0.5f / (float)mYSize); \
                    mXValues[0] = mCurFrame * mXGain[0] + xBias; \
                    mXValues[1] = (mCurFrame + 1) * mXGain[1] - xBias; \
                    mYValues[0] = mCurAnimation.mAnimation * mYGain[0] + yBias; \
                    mYValues[1] = (mCurAnimation.mAnimation + 1) * mYGain[1] - yBias
#define MAX_ANIMATION_BACKLOG 20

TextureCoordinates::TextureCoordinates()
{
  assert(0);
}
TextureCoordinates::TextureCoordinates(int const aXSize,
                                       int const aYSize, 
                                       int const aNumAnimations,
                                       std::vector<int> const &aNumFrames,
                                       std::vector<std::vector<float>> const &aAnimationSpeeds) : mCurFrame(0),
                                                                                                  mCurAnimation(),
                                                                                                  mPrevAnimations(),
                                                                                                  mTotalFrames(0),
                                                                                                  mXSize(aXSize),
                                                                                                  mYSize(aYSize),
                                                                                                  mCurTime(0),
                                                                                                  mAnimated(false),
                                                                                                  mCompleted(false),
                                                                                                  mSpeedModifiers(),
                                                                                                  mSpeeds(),
                                                                                                  mAnimations()
{
  // Validation
  if(aNumAnimations != aNumFrames.size() || aNumAnimations != aAnimationSpeeds.size())
  {
    assert(!"Number of animations vs the number of frames or animations speeds does not match.");
  }
  for(int i = 0; i < aNumAnimations; ++i)
  {
    if(aNumFrames[i] != aAnimationSpeeds[i].size())
    {
      assert(!"Number of frames vs interval between frames does not match.");
    }
  }
  
  // Initialization
  int maxFrames = 0;
  
  // Push back all frame data
  for(int i = 0; i < aNumAnimations; ++i)
  {
    mTotalFrames += aNumFrames[i];
    mAnimations.push_back(aNumFrames[i]);
    mSpeeds.push_back(aAnimationSpeeds[i]);
    mSpeedModifiers.push_back(1.0f);
    
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
  
  // Set base animation
  mPrevAnimations.push_back(mCurAnimation);
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
  
  mCurTime += aDT * mSpeedModifiers[mCurAnimation.mAnimation];
  
  // If it's time to change a frame
  while(mCurTime >= mSpeeds[mCurAnimation.mAnimation][mCurFrame])
  {
    mCurTime -= mSpeeds[mCurAnimation.mAnimation][mCurFrame];
    
    // Increase the frame
    ++mCurFrame;
    
    // Make sure we're within bounds
    if(mCurFrame >= mAnimations[mCurAnimation.mAnimation])
    {
      mCompleted = true;
      // If only run once, do not reset the frames.
      if(mCurAnimation.mBehavior == TextureCoordinateBehavior::RUN_ONCE)
      {
        mCurFrame = mAnimations[mCurAnimation.mAnimation] - 1;
      }
      else if(mCurAnimation.mBehavior == TextureCoordinateBehavior::RETURN_TO_PREVIOUS)
      {
        FindPreviousAnimation();
        SetCurrentAnimation(mPrevAnimations.back().mAnimation, mPrevAnimations.back().mBehavior);
      }
      else
      {
        mCurFrame = 0;
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
float TextureCoordinates::GetCurrentAnimationSpeed() const
{
  return mSpeeds[mCurAnimation.mAnimation][mCurFrame];
}

/**
 * @brief Get current animation index.
 * @return 
 */
int TextureCoordinates::GetCurrentAnimation() const
{
  return mCurAnimation.mAnimation;
}

/**
 * @brief Get previous animations info.
 * @return 
 */
TextureCoordinates::AnimationInfoContainer TextureCoordinates::GetPreviousAnimations() const
{
  return mPrevAnimations;
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
  return mAnimations[aAnimation];
}

/**
 * @brief Get number of frames for all animations.
 * @return Animation frame counts.
 */
std::vector<int> TextureCoordinates::GetAllAnimationFrameCounts() const
{
  return mAnimations;
}

/**
 * @brief Get current frame in animation.
 * @return Current frame.
 */
int TextureCoordinates::GetCurrentFrame() const
{
  return mCurFrame;
}

/**
 * @brief Get speed for animation
 * @param aAnimation The animation index.
 * @return 
 */
float TextureCoordinates::GetAnimationSpeed(int const aAnimation) const
{
  return mSpeedModifiers[aAnimation];
}

/**
 * @brief Get holds for animation.
 * @param aAnimation Animation to get holds for.
 * @return 
 */
TextureCoordinates::SpeedContainer const TextureCoordinates::GetAnimationHolds(int const aAnimation) const
{
  return mSpeeds[mCurAnimation.mAnimation];
}

/**
 * @brief Get holds for all animations.
 * @return All animation holds.
 */
std::vector<TextureCoordinates::SpeedContainer> const TextureCoordinates::GetAllAnimationHolds() const
{
  return mSpeeds;
}

/**
 * @brief Get time to a specific frame in animation.
 * @param aAnimation Animation to get frame from.
 * @param aFrame Frame to get time for.
 * @return Time to reach a particular frame.
 */
float TextureCoordinates::GetTimeToFrame(int const aAnimation, int const aFrame) const
{
  TextureCoordinates::SpeedContainer speeds = GetAnimationHolds(aAnimation);
  float ret = 0;
  
  if(aFrame >= speeds.size())
  {
    DebugLogPrint("Frame %d exceeds number of frames in animation %d.", aFrame, aAnimation);
    assert(!"Frame exceeds number of frames in animation.");
  }
  
  for(int i = 0; i < aFrame; ++i)
  {
    ret += speeds[i];
  }
  
  return ret;
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
 * @param aBehavior
 */
void TextureCoordinates::SetCurrentAnimation(int const aAnimation, TextureCoordinateBehavior const aBehavior)
{
  // So as to not reset the animation, we have an early out.
  if(mCurAnimation.mAnimation == aAnimation)
  {
    return;
  }
  // Assert in case of terribleness.
  else if(aAnimation >= (int)mAnimations.size())
  {
    assert(!"SetCurrentAnimation: aAnimation is larger than the number of animations.");
  }
  
  if(aAnimation != mCurAnimation.mAnimation)
  {
    mPrevAnimations.push_back(AnimationInfo(mCurAnimation.mAnimation, mCurAnimation.mBehavior));
    mCurAnimation.mAnimation = aAnimation;
    mCurAnimation.mBehavior = aBehavior;
    
    while(mPrevAnimations.size() > MAX_ANIMATION_BACKLOG)
      mPrevAnimations.pop_front();
  }
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
  if(mCurFrame > mAnimations[mCurAnimation.mAnimation])
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

  mCurAnimation.mAnimation = 0;
  mCurFrame = aFrameID;

  while(mCurFrame >= mAnimations[mCurAnimation.mAnimation])
  {
    mCurFrame -= mAnimations[mCurAnimation.mAnimation];
    ++mCurAnimation.mAnimation;
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
 * @brief Set animation behavior
 * @param aRunOnce
 */
void TextureCoordinates::SetBehavior(TextureCoordinateBehavior const aBehavior)
{
  mCurAnimation.mBehavior = aBehavior;
}

/**
 * @brief Set time before next frame (in seconds).
 * @param aSpeed
 */
void TextureCoordinates::SetCurrentAnimationSpeed(float const aSpeed)
{
  mSpeedModifiers[mCurAnimation.mAnimation] = aSpeed;
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
  mCurFrame = mAnimations[mCurAnimation.mAnimation] - 1;
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

/**
 * @brief Pop previous animation queue to get next animation.
 */
void TextureCoordinates::FindPreviousAnimation()
{
  while(mPrevAnimations.size() > 0 && mPrevAnimations.back().mBehavior == TextureCoordinateBehavior::RETURN_TO_PREVIOUS)
    mPrevAnimations.pop_back();
  if(mPrevAnimations.empty())
    mPrevAnimations.push_back(AnimationInfo(0, TextureCoordinateBehavior::CONTINUOUS));
}
