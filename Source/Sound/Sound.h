//
//  Sound.h
//  JFramework
//
//  Created by Jimmy Spencer on 10/2/13.
//
//

#ifndef __JFramework__Sound_H_
#define __JFramework__Sound_H_

#include "Common.h"

struct Sound
{
  static int const INFINITE_LOOPS = -1;
  static int const ONE_LOOP = 0;
  
  HashString      mName;
  unsigned char*  mData;
  unsigned int    mPos;
  unsigned int    mLength;
  
  Sound();
  Sound(HashString const &aFilename);
  virtual ~Sound();
  
  virtual void Play(int const aNumLoops) = 0;
  virtual void Play(int const aNumLoops, int const aTime) = 0;
  virtual void FadeIn(int const aNumLoops, int const aTime) = 0;
  virtual void FadeIn(int const aNumLoops, int const aFadeTime, int const aPlayTime) = 0;
  
  virtual void SetVolume(int const aVolume) = 0;
  
  virtual void Resume() = 0;
  virtual void Pause() = 0;

  virtual void Stop() = 0;
  virtual void FadeOut(int const aTime) = 0;
};

#endif /* defined(__JFramework__Sound__) */
