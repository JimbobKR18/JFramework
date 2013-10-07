//
//  PCSound.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 10/2/13.
//
//

#include "PCSound.h"

PCSound::PCSound() : Sound()
{
}

PCSound::PCSound(std::string const &aFilename) : Sound(aFilename)
{
}

PCSound::~PCSound()
{
}

void PCSound::Play()
{
}

void PCSound::Mix(void *aUnused, unsigned char *aStream, unsigned int aLength)
{
  unsigned int amount = mLength - mPos;
  if(amount > aLength)
    amount = aLength;
  
  SDL_MixAudio(aStream, &mData[mPos], amount, SDL_MIX_MAXVOLUME);
  mPos += amount;
}