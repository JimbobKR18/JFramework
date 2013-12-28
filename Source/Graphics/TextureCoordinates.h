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
  int                         mNumAnimations;
  float                       mXGain;
  float                       mYGain;
  float                       mCurTime;
  float                       mXValues[2];
  float                       mYValues[2];
  bool                        mAnimated;
  bool                        mCompleted;
  
  // <currentAnimation, numberofFrames>
  std::map<int, int>          mAnimations;
  typedef std::pair<int,int>  AnimationData;
  
public:
  TextureCoordinates();
  TextureCoordinates(int const aNumAnimations, std::vector<int> const aNumFrames);
  
  ~TextureCoordinates();
  
  void  Update(float aDT);
  float GetXValue(int aValue) const;
  float GetYValue(int aValue) const;
  int   GetCurrentAnimation() const;
  bool  GetCompleted() const;
  void  SetCurrentAnimation(int aAnimation);
  void  SetCurrentFrame(int aFrame);
  void  SetAnimated(bool aAnimated);
  void  Reset();
};

#endif /* defined(__JFramework__TextureCoordinates__) */
