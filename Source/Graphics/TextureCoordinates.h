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

class TextureCoordinates
{
public:
  typedef std::vector<float> SpeedContainer;
  typedef SpeedContainer::const_iterator SpeedConstIT;
  
private:
  int                         mCurFrame;
  int                         mCurAnimation;
  int                         mTotalFrames;
  int                         mXSize;
  int                         mYSize;
  float                       mCurTime;
  float                       mXGain[2];
  float                       mYGain[2];
  float                       mXValues[2];
  float                       mYValues[2];
  bool                        mAnimated;
  bool                        mCompleted;
  bool                        mRunOnce;
  
  // <currentAnimation, speeds>
  std::map<int, float>                   mSpeedModifiers;
  typedef std::pair<int, float>          SpeedModifier;
  std::map<int, SpeedContainer>          mSpeeds;
  typedef std::pair<int, SpeedContainer> AnimationSpeed;
  
  // <currentAnimation, numberofFrames>
  std::map<int, int>          mAnimations;
  typedef std::pair<int, int> AnimationData;
  
public:
  TextureCoordinates();
  TextureCoordinates(int const aXSize, int const aYSize, int const aNumAnimations, 
                     std::vector<int> const &aNumFrames, std::vector<std::vector<float>> const &aAnimationSpeeds);
  
  ~TextureCoordinates();
  
  void  Update(float aDT);

  // GETTERS
  float GetXValue(int const aIndex) const;
  float GetYValue(int const aIndex) const;
  float GetCurrentAnimationSpeed() const;
  int   GetCurrentAnimation() const;
  int   GetNumberOfAnimations() const;
  int   GetTotalFrames() const;
  int   GetAnimationFrameCounts(int const aAnimation) const;
  float GetAnimationSpeed(int const aAnimation) const;
  SpeedContainer const GetAnimationHolds(int const aAnimation) const;
  bool  GetCompleted() const;
  bool  GetAnimated() const;

  // SETTERS
  void  SetCurrentAnimation(int const aAnimation);
  void  SetCurrentFrame(int const aFrame);
  void  SetFrameByID(int const aFrameID);
  void  SetAnimated(bool const aAnimated);
  void  SetRunOnce(bool const aRunOnce);
  void  SetCurrentAnimationSpeed(float const aSpeed);
  void  SetXGain(int const aIndex, float const aX);
  void  SetYGain(int const aIndex, float const aY);

  // HELPERS
  void  Finish();
  void  Reset();
};

#endif /* defined(__JFramework__TextureCoordinates__) */
