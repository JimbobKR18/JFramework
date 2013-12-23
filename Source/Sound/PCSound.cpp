//
//  PCSound.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 10/2/13.
//
//

#include "PCSound.h"
#include "Common.h"

PCSound::PCSound() : Sound()
{
}

PCSound::PCSound(std::string const &aFilename) : Sound(aFilename)
{
  mChunk = Mix_LoadWAV(aFilename.c_str());
  if(!mChunk)
  {
    printf("Mix_QuickLoad_WAV: %s\n", Mix_GetError());
    assert(!"Mix_Quickload_WAV failed, aborting.");
  }
}

PCSound::~PCSound()
{
}

void PCSound::Play()
{
  if(Mix_PlayChannel(-1, mChunk, 0) == -1)
  {
    printf("Mix_PlayChannel: %s\n", Mix_GetError());
    assert(!"Mix_PlayChannel failed, aborting.");
  }
}

void PCSound::Play(int aTime)
{
  if(Mix_PlayChannelTimed(-1, mChunk, 0, aTime) == -1)
  {
    printf("Mix_PlayChannelTimed: %s\n", Mix_GetError());
    assert(!"Mix_PlayChannelTimed failed, aborting.");
  }
}

void PCSound::FadeIn(int aTime)
{
  if(Mix_FadeInChannel(-1, mChunk, 0, aTime) == -1)
  {
    printf("Mix_FadeInChannel: %s\n", Mix_GetError());
    assert(!"Mix_FadeInChannel failed, aborting.");
  }
}

void PCSound::FadeIn(int aFadeTime, int aPlayTime)
{
  if(Mix_FadeInChannelTimed(-1, mChunk, 0, aFadeTime, aPlayTime) == -1)
  {
    printf("Mix_FadeInChannelTimed: %s\n", Mix_GetError());
    assert(!"Mix_FadeInChannelTimed failed, aborting.");
  }
}
