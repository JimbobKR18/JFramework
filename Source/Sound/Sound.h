//
//  Sound.h
//  JFramework
//
//  Created by Jimmy Spencer on 10/2/13.
//
//

#ifndef __JFramework__Sound_H_
#define __JFramework__Sound_H_

#include <string>

struct Sound
{
  std::string     mName;
  unsigned char*  mData;
  unsigned int    mPos;
  unsigned int    mLength;
  
  Sound();
  Sound(std::string const &aFilename);
  virtual ~Sound();
  
  virtual void Play() = 0;
  virtual void Play(int const aTime) = 0;
  virtual void FadeIn(int const aTime) = 0;
  virtual void FadeIn(int const aFadeTime, int const aPlayTime) = 0;
  
  virtual void SetVolume(int const aVolume) = 0;
  
  virtual void Resume() = 0;
  virtual void Pause() = 0;

  virtual void Stop() = 0;
  virtual void FadeOut(int const aTime) = 0;
};

#endif /* defined(__JFramework__Sound__) */
