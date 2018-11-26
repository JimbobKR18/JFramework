//
//  TextureCoordinates.h
//  JFramework
//
//  Created by Jimmy Spencer on 12/30/12.
//  The idea: A class designed to keep track
//  of the frames for an animation
//

#ifndef __JFramework__TextureCoordinates__
#define __JFramework__TextureCoordinates__

#include "Common.h"

enum TextureCoordinateBehavior
{
  CONTINUOUS = 0,
  RUN_ONCE,
  RETURN_TO_PREVIOUS
};

struct AnimationInfo
{
  int mAnimation;
  TextureCoordinateBehavior mBehavior;
  
  AnimationInfo() : mAnimation(0), mBehavior(TextureCoordinateBehavior::CONTINUOUS) {}
  AnimationInfo(AnimationInfo const &aAnimationInfo) : mAnimation(aAnimationInfo.mAnimation), mBehavior(aAnimationInfo.mBehavior) {}
  AnimationInfo(int aAnimation, TextureCoordinateBehavior const aBehavior) : 
    mAnimation(aAnimation), mBehavior(aBehavior) {}
  virtual ~AnimationInfo() {}
};

class TextureCoordinates
{
public:
  typedef std::vector<float> SpeedContainer;
  typedef SpeedContainer::const_iterator SpeedConstIT;
  typedef std::list<AnimationInfo> AnimationInfoContainer;
  typedef AnimationInfoContainer::const_iterator AnimationInfoConstIT;
  
private:
  int                         mCurFrame;
  AnimationInfo               mCurAnimation;
  AnimationInfoContainer      mPrevAnimations;
  int                         mTotalFrames;
  int                         mXSize;
  int                         mYSize;
  int                         mMaxFrames;
  float                       mCurTime;
  double                      mXGain[2];
  double                      mYGain[2];
  double                      mXValues[2];
  double                      mYValues[2];
  double                      mBias[2];
  bool                        mAnimated;
  bool                        mCompleted;
  
  // <speeds>
  std::vector<float>            mSpeedModifiers;
  std::vector<SpeedContainer>   mSpeeds;
  
  // <numberofFrames>
  std::vector<int>              mAnimations;
  
public:
  TextureCoordinates();
  TextureCoordinates(TextureCoordinates const &aTextureCoordinates);
  TextureCoordinates(int const aXSize, int const aYSize, int const aNumAnimations, 
                     std::vector<int> const &aNumFrames, std::vector<std::vector<float>> const &aAnimationSpeeds);
  
  ~TextureCoordinates();
  
  void  Update(float aDT);

  // GETTERS
  double GetXValue(int const aIndex) const;
  double GetYValue(int const aIndex) const;
  int    GetXSize() const;
  int    GetYSize() const;
  double GetBias(int const aIndex) const;
  float GetCurrentAnimationSpeed() const;
  int   GetCurrentAnimation() const;
  AnimationInfoContainer GetPreviousAnimations() const;
  int   GetNumberOfAnimations() const;
  int   GetTotalFrames() const;
  int   GetAnimationFrameCounts(int const aAnimation) const;
  std::vector<int> GetAllAnimationFrameCounts() const;
  int   GetCurrentFrame() const;
  float GetAnimationSpeed(int const aAnimation) const;
  SpeedContainer const GetAnimationHolds(int const aAnimation) const;
  std::vector<SpeedContainer> const GetAllAnimationHolds() const;
  float GetTimeToFrame(int const aAnimation, int const aFrame) const;
  bool  GetCompleted() const;
  bool  GetAnimated() const;

  // SETTERS
  void  SetCurrentAnimation(int const aAnimation, TextureCoordinateBehavior const aBehavior);
  void  SetCurrentFrame(int const aFrame);
  void  SetFrameByID(int const aFrameID);
  void  SetAnimated(bool const aAnimated);
  void  SetBehavior(TextureCoordinateBehavior const aBehavior);
  void  SetCurrentAnimationSpeed(float const aSpeed);
  void  SetXGain(int const aIndex, double const aX);
  void  SetYGain(int const aIndex, double const aY);
  void  SetBias(int const aIndex, double const aBias);

  // HELPERS
  void  Finish();
  void  Reset();
  
private:
  void  FindPreviousAnimation();
};

#endif /* defined(__JFramework__TextureCoordinates__) */
