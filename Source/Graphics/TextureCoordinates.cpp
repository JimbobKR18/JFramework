//
//  TextureCoordinates.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/30/12.
//
//

#include "TextureCoordinates.h"
#define MAX_ANIMATION_BACKLOG 20

TextureCoordinates::TextureCoordinates()
{
  assert(0);
}

TextureCoordinates::TextureCoordinates(TextureCoordinates const &aTextureCoordinates) : mCurFrame(aTextureCoordinates.mCurFrame),
  mCurAnimation(aTextureCoordinates.mCurAnimation), mPrevAnimations(aTextureCoordinates.mPrevAnimations),
  mTotalFrames(aTextureCoordinates.mTotalFrames), mXSize(aTextureCoordinates.mXSize),
  mYSize(aTextureCoordinates.mYSize), mMaxFrames(aTextureCoordinates.mMaxFrames),
  mCurTime(aTextureCoordinates.mCurTime), mAnimated(aTextureCoordinates.mAnimated),
  mCompleted(aTextureCoordinates.mCompleted), mAnimations(aTextureCoordinates.mAnimations)
{
  mXGain[0] = aTextureCoordinates.mXGain[0];
  mXGain[1] = aTextureCoordinates.mXGain[1];
  mYGain[0] = aTextureCoordinates.mYGain[0];
  mYGain[1] = aTextureCoordinates.mYGain[1];
  mXValues[0] = aTextureCoordinates.mXValues[0];
  mXValues[1] = aTextureCoordinates.mXValues[1];
  mYValues[0] = aTextureCoordinates.mYValues[0];
  mYValues[1] = aTextureCoordinates.mYValues[1];
  mBias[0] = aTextureCoordinates.mBias[0];
  mBias[1] = aTextureCoordinates.mBias[1];
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
                                                                                                  mMaxFrames(0),
                                                                                                  mCurTime(0),
                                                                                                  mAnimated(false),
                                                                                                  mCompleted(false),
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
  mMaxFrames = 0;
  for(int i = 0; i < aNumAnimations; ++i)
  {
    // Need to figure out each frame size
    if(aNumFrames[i] > mMaxFrames)
    {
      mMaxFrames = aNumFrames[i];
    }
  }
  
  // Push back all frame data
  for(int i = 0; i < aNumAnimations; ++i)
  {
    mTotalFrames += aNumFrames[i];
    std::vector<int> frames;
    for(int j = 0; j < aNumFrames[i]; ++j)
    {
      frames.push_back(j + (mMaxFrames * i));
    }
    
    AnimationInfo *animationInfo = new AnimationInfo(i, 1.0f, aAnimationSpeeds[i], frames, 0);
    mAnimations.push_back(animationInfo);
  }
  
  // Figure out the gain per step of animation
  mXGain[0] = mXGain[1] = 1.0f / (double)mMaxFrames;
  mYGain[0] = mYGain[1] = 1.0f / (double)aNumAnimations;
  mBias[0] = 0;
  mBias[1] = 0;
  
  // Set all values to starting positions
  SetFrames();
  
  // Set base animation
  mPrevAnimations.push_back(mCurAnimation);
}

TextureCoordinates::TextureCoordinates(int const aXSize, int const aYSize, int const aNumColumns, 
                                       int const aNumRows, std::vector<AnimationInfo*> const &aAnimations) :  mCurFrame(0),
                                                                                                              mCurAnimation(),
                                                                                                              mPrevAnimations(),
                                                                                                              mTotalFrames(0),
                                                                                                              mXSize(aXSize),
                                                                                                              mYSize(aYSize),
                                                                                                              mMaxFrames(aNumColumns),
                                                                                                              mCurTime(0),
                                                                                                              mAnimated(false),
                                                                                                              mCompleted(false),
                                                                                                              mAnimations(aAnimations)
{
  // Total frames calc.
  mTotalFrames = aNumColumns * aNumRows;
  
  // Figure out the gain per step of animation
  mXGain[0] = mXGain[1] = 1.0f / (double)mMaxFrames;
  mYGain[0] = mYGain[1] = 1.0f / (double)aNumRows;
  mBias[0] = 0;
  mBias[1] = 0;
  
  SetFrames();
  
  // Set base animation
  mPrevAnimations.push_back(mCurAnimation);
}

TextureCoordinates::~TextureCoordinates()
{
  for(std::vector<AnimationInfo*>::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
  {
    delete *it;
  }
  mAnimations.clear();
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
  
  AnimationInfo *animationInfo = mAnimations[mCurAnimation.mAnimation];
  mCurTime += aDT * animationInfo->mSpeedModifier;
  
  // If it's time to change a frame
  while(mCurTime >= animationInfo->mSpeeds[mCurFrame])
  {
    mCurTime -= animationInfo->mSpeeds[mCurFrame];
    
    // Increase the frame
    ++mCurFrame;
    
    // Make sure we're within bounds
    if(mCurFrame >= animationInfo->mFrames.size())
    {
      mCompleted = true;
      // If only run once, do not reset the frames.
      if(mCurAnimation.mBehavior == TextureCoordinateBehavior::RUN_ONCE)
      {
        mCurFrame = animationInfo->mFrames.size() - 1;
      }
      else if(mCurAnimation.mBehavior == TextureCoordinateBehavior::RETURN_TO_PREVIOUS)
      {
        FindPreviousAnimation();
        SetCurrentAnimation(mPrevAnimations.back().mAnimation, mPrevAnimations.back().mBehavior);
      }
      else
      {
        mCurFrame = animationInfo->mLoopStartFrame;
      }
    }
    
    // Set positions of coordinates
    SetFrames();
  }
}

/**
 * @brief Get current x coordinate.
 * @param aIndex 0 for left, 1 for right.
 * @return 
 */
double TextureCoordinates::GetXValue(int const aIndex) const
{
  return mXValues[aIndex];
}

/**
 * @brief Get current y coordinate.
 * @param aIndex 0 for top, 1 for bottom.
 * @return 
 */
double TextureCoordinates::GetYValue(int const aIndex) const
{
  return mYValues[aIndex];
}

/**
 * @brief Return X size of texture.
 * @return X size of texture.
 */
int TextureCoordinates::GetXSize() const
{
  return mXSize;
}

/**
 * @brief Return Y size of texture.
 * @return Y size of texture.
 */
int TextureCoordinates::GetYSize() const
{
  return mYSize;
}

/**
 * @brief Get bias for axis.
 * @param aIndex 0 for X, 1 for Y.
 * @return Bias.
 */
double TextureCoordinates::GetBias(int const aIndex) const
{
  return mBias[aIndex];
}

/**
 * @brief Get animation speed.
 * @return 
 */
float TextureCoordinates::GetCurrentAnimationSpeed() const
{
  AnimationInfo *animationInfo = mAnimations[mCurAnimation.mAnimation];
  return animationInfo->mSpeeds[mCurFrame];
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
TextureCoordinates::AnimationPlayInfoContainer TextureCoordinates::GetPreviousAnimations() const
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
 * @brief Get max frames per row.
 * @return 
 */
int TextureCoordinates::GetMaxFrames() const
{
  return mMaxFrames;
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
  AnimationInfo *animationInfo = mAnimations[mCurAnimation.mAnimation];
  return animationInfo->mFrames.size();
}

/**
 * @brief Get number of frames for all animations.
 * @return Animation frame counts.
 */
std::vector<int> TextureCoordinates::GetAllAnimationFrameCounts() const
{
  std::vector<int> ret;
  for(std::vector<AnimationInfo*>::const_iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
  {
    ret.push_back((*it)->mFrames.size());
  }
  return ret;
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
  AnimationInfo *animationInfo = mAnimations[mCurAnimation.mAnimation];
  return animationInfo->mSpeedModifier;
}

/**
 * @brief Get holds for animation.
 * @param aAnimation Animation to get holds for.
 * @return 
 */
std::vector<float> const TextureCoordinates::GetAnimationHolds(int const aAnimation) const
{
  AnimationInfo *animationInfo = mAnimations[mCurAnimation.mAnimation];
  return animationInfo->mSpeeds;
}

/**
 * @brief Get holds for all animations.
 * @return All animation holds.
 */
std::vector<std::vector<float>> const TextureCoordinates::GetAllAnimationHolds() const
{
  std::vector<std::vector<float>> ret;
  for(std::vector<AnimationInfo*>::const_iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
  {
    ret.push_back((*it)->mSpeeds);
  }
  return ret;
}

/**
 * @brief Get all animations.
 * @return All animations.
 */
std::vector<AnimationInfo*> const TextureCoordinates::GetAllAnimations() const
{
  return mAnimations;
}

/**
 * @brief Get time to a specific frame in animation.
 * @param aAnimation Animation to get frame from.
 * @param aFrame Frame to get time for.
 * @return Time to reach a particular frame.
 */
float TextureCoordinates::GetTimeToFrame(int const aAnimation, int const aFrame) const
{
  std::vector<float> speeds = GetAnimationHolds(aAnimation);
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
  
  // Push previous animation information.
  mPrevAnimations.push_back(AnimationPlayInfo(mCurAnimation.mAnimation, mCurAnimation.mBehavior));
  mCurAnimation.mAnimation = aAnimation;
  mCurAnimation.mBehavior = aBehavior;
    
  // Purge queue.
  while(mPrevAnimations.size() > MAX_ANIMATION_BACKLOG)
    mPrevAnimations.pop_front();
    
  // Reset data.  
  mCurFrame = 0;
  mCompleted = false;
  
  // Set positions of coordinates
  SetFrames();
}

/**
 * @brief Set current frame in animation
 * @param aFrame
 */
void TextureCoordinates::SetCurrentFrame(int const aFrame)
{
  AnimationInfo *animationInfo = mAnimations[mCurAnimation.mAnimation];
  if(mCurFrame >= animationInfo->mFrames.size())
  {
    assert(!"SetCurrentFrame: aFrame is larger than number of frames in animation");
  }

  mCurFrame = aFrame;
  
  // Set positions of coordinates
  SetFrames();
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

  AnimationInfo *animationInfo = mAnimations[mCurAnimation.mAnimation];
  while(mCurFrame >= animationInfo->mFrames.size())
  {
    mCurFrame -= animationInfo->mFrames.size();
    ++mCurAnimation.mAnimation;
    animationInfo = mAnimations[mCurAnimation.mAnimation];
  }

  // Set positions of coordinates
  SetFrames();
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
  AnimationInfo *animationInfo = mAnimations[mCurAnimation.mAnimation];
  animationInfo->mSpeedModifier = aSpeed;
}

/**
 * @brief Set pace in which the x coordinate should gain per frame.
 * @param aIndex 0 for left, 1 for right.
 * @param aX The gain.
 */
void TextureCoordinates::SetXGain(int const aIndex, double const aXGain)
{
  mXGain[aIndex] = aXGain;
}

/**
 * @brief Set pace in which the y coordinate should gain per animation.
 * @param aIndex 0 for top, 1 for bottom.
 * @param aY The gain.
 */
void TextureCoordinates::SetYGain(int const aIndex, double const aYGain)
{
  mYGain[aIndex] = aYGain;
}

/**
 * @brief Set bias on texture coordinates.
 * @param aIndex 0 for X, 1 for Y.
 * @param aBias The bias.
 */
void TextureCoordinates::SetBias(int const aIndex, double const aBias)
{
  mBias[aIndex] = aBias;
}

/**
 * @brief Finish current animation.
 */
void TextureCoordinates::Finish()
{
  // Set to last frame.
  AnimationInfo *animationInfo = mAnimations[mCurAnimation.mAnimation];
  mCurFrame = animationInfo->mFrames.size() - 1;
  // Set positions of coordinates
  SetFrames();
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
    mPrevAnimations.push_back(AnimationPlayInfo());
}

/**
 * @brief Set frames based on current animation / frame.
 */
void TextureCoordinates::SetFrames()
{
  AnimationInfo *animationInfo = mAnimations[mCurAnimation.mAnimation];
  int frameIndex = animationInfo->mFrames[mCurFrame] % mMaxFrames;
  int animationIndex = animationInfo->mFrames[mCurFrame] / mMaxFrames;
  mXValues[0] = frameIndex * mXGain[0] + mBias[0];
  mXValues[1] = (frameIndex + 1) * mXGain[1] - mBias[0];
  mYValues[0] = animationIndex * mYGain[0] + mBias[1];
  mYValues[1] = (animationIndex + 1) * mYGain[1] - mBias[1];
}
