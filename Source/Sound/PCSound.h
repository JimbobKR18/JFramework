//
//  PCSound.h
//  JFramework
//
//  Created by Jimmy Spencer on 10/2/13.
//
//

#ifndef __JFramework__PCSound__
#define __JFramework__PCSound__

#include "Sound.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class PCSound : public Sound
{
private:
  Mix_Chunk *mChunk;
public:
  PCSound();
  PCSound(std::string const &aFilename);
  virtual ~PCSound();
  
  virtual void Play();
  virtual void Play(int aTime);
  virtual void FadeIn(int aTime);
  virtual void FadeIn(int aFadeTime, int aPlayTime);
};

#endif /* defined(__JFramework__PCSound__) */
