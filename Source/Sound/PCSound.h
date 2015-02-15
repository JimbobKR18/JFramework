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
#if defined(_WIN32)
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

class PCSound : public Sound
{
private:
  Mix_Chunk*  mChunk;
  int         mChannel;
public:
  PCSound();
  PCSound(std::string const &aFilename);
  virtual ~PCSound();
  
  virtual void Play();
  virtual void Play(int aTime);
  virtual void FadeIn(int aTime);
  virtual void FadeIn(int aFadeTime, int aPlayTime);

  virtual void Stop();
};

#endif /* defined(__JFramework__PCSound__) */
