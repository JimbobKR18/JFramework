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
#include <SDL/SDL_audio.h>

class PCSound : public Sound
{
private:
public:
  PCSound();
  PCSound(std::string const &aFilename);
  virtual ~PCSound();
  
  virtual void Play();
  virtual void Mix(void *aUnused, unsigned char *aStream, unsigned int aLength);
};

#endif /* defined(__JFramework__PCSound__) */
