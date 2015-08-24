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
private:
  int                         mCurFrame;
  int                         mCurAnimation;
  int                         mTotalFrames;
  int                         mXSize;
  int                         mYSize;
  float                       mSpeed;
  float                       mCurTime;
  float                       mXGain;
  float                       mYGain;
  float                       mXValues[2];
  float                       mYValues[2];
  bool                        mAnimated;
  bool                        mCompleted;
  bool                        mRunOnce;
  
  // <currentAnimation, numberofFrames>
  std::map<int, int>          mAnimations;
  typedef std::pair<int,int>  AnimationData;
  
public:
  TextureCoordinates();
  TextureCoordinates(int const aXSize, int const aYSize, int const aNumAnimations, std::vector<int> const aNumFrames, float aAnimationSpeed);
  
  ~TextureCoordinates();
  
  void  Update(float aDT);

  // GETTERS
  float GetXValue(int const aIndex) const;
  float GetYValue(int const aIndex) const;
  float GetAnimationSpeed() const;
  int   GetCurrentAnimation() const;
  int   GetNumberOfAnimations() const;
  int   GetTotalFrames() const;
  int   GetAnimationFrameCounts(int const aAnimation) const;
  bool  GetCompleted() const;
  bool  GetAnimated() const;

  // SETTERS
  void  SetCurrentAnimation(int const aAnimation);
  void  SetCurrentFrame(int const aFrame);
  void  SetFrameByID(int const aFrameID);
  void  SetAnimated(bool const aAnimated);
  void  SetRunOnce(bool const aRunOnce);
  void  SetSpeed(float const aSpeed);
  void  SetXGain(float const aX);
  void  SetYGain(float const aY);

  // HELPERS
  void  Finish();
  void  Reset();
};

#endif /* defined(__JFramework__TextureCoordinates__) */
