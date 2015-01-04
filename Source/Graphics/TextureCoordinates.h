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
  
  // <currentAnimation, numberofFrames>
  std::map<int, int>          mAnimations;
  typedef std::pair<int,int>  AnimationData;
  
public:
  TextureCoordinates();
  TextureCoordinates(int const aXSize, int const aYSize, int const aNumAnimations, std::vector<int> const aNumFrames, float aAnimationSpeed);
  
  ~TextureCoordinates();
  
  void  Update(float aDT);

  // GETTERS
  float GetXValue(int aValue) const;
  float GetYValue(int aValue) const;
  float GetAnimationSpeed() const;
  int   GetCurrentAnimation() const;
  int   GetNumberofAnimations() const;
  int   GetTotalFrames() const;
  int   GetAnimationFrameCounts(int const aAnimation) const;
  bool  GetCompleted() const;
  bool  GetAnimated() const;

  // SETTERS
  void  SetCurrentAnimation(int aAnimation);
  void  SetCurrentFrame(int aFrame);
  void  SetFrameByID(int aFrameID);
  void  SetAnimated(bool aAnimated);
  void  SetSpeed(float aSpeed);

  // HELPERS
  void  Reset();
};

#endif /* defined(__JFramework__TextureCoordinates__) */
