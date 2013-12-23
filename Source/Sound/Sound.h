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
  virtual void Play(int aTime) = 0;
  virtual void FadeIn(int aTime) = 0;
  virtual void FadeIn(int aFadeTime, int aPlayTime) = 0;
};

#endif /* defined(__JFramework__Sound__) */
