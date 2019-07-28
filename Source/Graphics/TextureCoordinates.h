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

struct AnimationPlayInfo
{
  int mAnimation;
  TextureCoordinateBehavior mBehavior;
  
  AnimationPlayInfo() : mAnimation(0), mBehavior(TextureCoordinateBehavior::CONTINUOUS) {}
  AnimationPlayInfo(AnimationPlayInfo const &aAnimationInfo) : mAnimation(aAnimationInfo.mAnimation), mBehavior(aAnimationInfo.mBehavior) {}
  AnimationPlayInfo(int aAnimation, TextureCoordinateBehavior const aBehavior) : 
    mAnimation(aAnimation), mBehavior(aBehavior) {}
  virtual ~AnimationPlayInfo() {}
};

struct AnimationInfo
{
  int mId;
  float mSpeedModifier;
  std::vector<float> mSpeeds;
  std::vector<int> mFrames;
  int mLoopStartFrame;
  
  AnimationInfo() : mId(0), mSpeedModifier(1), mSpeeds(), mFrames(), mLoopStartFrame(0) {}
  AnimationInfo(AnimationInfo const &aAnimationInfo) : mId(aAnimationInfo.mId), mSpeedModifier(aAnimationInfo.mSpeedModifier),
    mSpeeds(aAnimationInfo.mSpeeds), mFrames(aAnimationInfo.mFrames), mLoopStartFrame(aAnimationInfo.mLoopStartFrame) {}
  AnimationInfo(int const &aId, float const &aSpeedModifier, std::vector<float> const &aSpeeds, std::vector<int> const &aFrames, 
    int const &aLoopStartFrame) : 
    mId(aId), mSpeedModifier(aSpeedModifier), mSpeeds(aSpeeds), mFrames(aFrames) , mLoopStartFrame(aLoopStartFrame)
  {
    assert(mSpeeds.size() == mFrames.size());
  }
  virtual ~AnimationInfo() 
  {
    mSpeeds.clear();
    mFrames.clear();
  }
};

class TextureCoordinates
{
public:
  typedef std::list<AnimationPlayInfo> AnimationPlayInfoContainer;
  typedef AnimationPlayInfoContainer::const_iterator AnimationPlayInfoConstIT;
  
private:
  int                         mCurFrame;
  AnimationPlayInfo           mCurAnimation;
  AnimationPlayInfoContainer  mPrevAnimations;
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
  
  // <id, animation>
  std::vector<AnimationInfo*> mAnimations;
  
public:
  TextureCoordinates();
  TextureCoordinates(TextureCoordinates const &aTextureCoordinates);
  TextureCoordinates(int const aXSize, int const aYSize, int const aNumAnimations, 
                     std::vector<int> const &aNumFrames, std::vector<std::vector<float>> const &aAnimationSpeeds);
  TextureCoordinates(int const aXSize, int const aYSize, int const aNumColumns, int const aNumRows, std::vector<AnimationInfo*> const &aAnimations);
  
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
  AnimationPlayInfoContainer GetPreviousAnimations() const;
  int   GetNumberOfAnimations() const;
  int   GetMaxFrames() const;
  int   GetTotalFrames() const;
  int   GetAnimationFrameCounts(int const aAnimation) const;
  std::vector<int> GetAllAnimationFrameCounts() const;
  int   GetCurrentFrame() const;
  float GetAnimationSpeed(int const aAnimation) const;
  std::vector<float> const GetAnimationHolds(int const aAnimation) const;
  std::vector<std::vector<float>> const GetAllAnimationHolds() const;
  std::vector<AnimationInfo*> const GetAllAnimations() const;
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
  void  SetFrames();
};

#endif /* defined(__JFramework__TextureCoordinates__) */
