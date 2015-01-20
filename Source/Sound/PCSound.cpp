//
//  PCSound.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 10/2/13.
//
//

#include "PCSound.h"
#include "Common.h"

PCSound::PCSound() : Sound(), mChannel(-1)
{
}

PCSound::PCSound(std::string const &aFilename) : Sound(aFilename), mChannel(-1)
{
  mChunk = Mix_LoadWAV(aFilename.c_str());
  if(!mChunk)
  {
    DebugLogPrint("Mix_QuickLoad_WAV: %s\n", Mix_GetError());
    assert(!"Mix_Quickload_WAV failed, aborting.");
  }
}

PCSound::~PCSound()
{
}

void PCSound::Play()
{
  mChannel = Mix_PlayChannel(-1, mChunk, 0);
  if(mChannel == -1)
  {
    DebugLogPrint("Mix_PlayChannel: %s\n", Mix_GetError());
    assert(!"Mix_PlayChannel failed, aborting.");
  }
}

void PCSound::Play(int aTime)
{
  mChannel = Mix_PlayChannelTimed(-1, mChunk, 0, aTime);
  if(mChannel == -1)
  {
    DebugLogPrint("Mix_PlayChannelTimed: %s\n", Mix_GetError());
    assert(!"Mix_PlayChannelTimed failed, aborting.");
  }
}

void PCSound::FadeIn(int aTime)
{
  mChannel = Mix_FadeInChannel(-1, mChunk, 0, aTime);
  if(mChannel == -1)
  {
    DebugLogPrint("Mix_FadeInChannel: %s\n", Mix_GetError());
    assert(!"Mix_FadeInChannel failed, aborting.");
  }
}

void PCSound::FadeIn(int aFadeTime, int aPlayTime)
{
  mChannel = Mix_FadeInChannelTimed(-1, mChunk, 0, aFadeTime, aPlayTime);
  if(mChannel == -1)
  {
    DebugLogPrint("Mix_FadeInChannelTimed: %s\n", Mix_GetError());
    assert(!"Mix_FadeInChannelTimed failed, aborting.");
  }
}

void PCSound::Stop()
{
  Mix_HaltChannel(mChannel);
  mChannel = -1;
}
