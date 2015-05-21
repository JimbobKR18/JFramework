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

/**
 * @brief Plays sound all the way through.
 */
void PCSound::Play()
{
  mChannel = Mix_PlayChannel(-1, mChunk, 0);
  if(mChannel == -1)
  {
    DebugLogPrint("Mix_PlayChannel: %s\n", Mix_GetError());
    assert(!"Mix_PlayChannel failed, aborting.");
  }
}

/**
 * @brief Play sound for a set amount of time.
 * @param aTime Time in millis.
 */
void PCSound::Play(int const aTime)
{
  mChannel = Mix_PlayChannelTimed(-1, mChunk, 0, aTime);
  if(mChannel == -1)
  {
    DebugLogPrint("Mix_PlayChannelTimed: %s\n", Mix_GetError());
    assert(!"Mix_PlayChannelTimed failed, aborting.");
  }
}

/**
 * @brief Fade in a sound.
 * @param aTime Time in millis.
 */
void PCSound::FadeIn(int const aTime)
{
  mChannel = Mix_FadeInChannel(-1, mChunk, 0, aTime);
  if(mChannel == -1)
  {
    DebugLogPrint("Mix_FadeInChannel: %s\n", Mix_GetError());
    assert(!"Mix_FadeInChannel failed, aborting.");
  }
}

/**
 * @brief Fade in a sound.
 * @param aTime Time in millis.
 */
void PCSound::FadeIn(int const aFadeTime, int const aPlayTime)
{
  mChannel = Mix_FadeInChannelTimed(-1, mChunk, 0, aFadeTime, aPlayTime);
  if(mChannel == -1)
  {
    DebugLogPrint("Mix_FadeInChannelTimed: %s\n", Mix_GetError());
    assert(!"Mix_FadeInChannelTimed failed, aborting.");
  }
}

/**
 * @brief Set volume for this sound.
 * @param aVolume The volume, from 0 to MAX_VOLUME(128)
 */
void PCSound::SetVolume(int const aVolume)
{
  if(mChannel != -1)
  {
    Mix_Volume(mChannel, aVolume);
  }
}

/**
 * @brief Resumes a sound
 */
void PCSound::Resume()
{
  if(mChannel != -1)
  {
    Mix_Resume(mChannel);
  }
}

/**
 * @brief Pauses a sound.
 */
void PCSound::Pause()
{
  if(mChannel != -1)
  {
    Mix_Pause(mChannel);
  }
}

/**
 * @brief Stop a sound.
 */
void PCSound::Stop()
{
  Mix_HaltChannel(mChannel);
  mChannel = -1;
}

/**
 * @brief Fade out a sound.
 * @param aTime Time in millis.
 */
void PCSound::FadeOut(int const aTime)
{
  mChannel = Mix_FadeOutChannel(mChannel, aTime);
  mChannel = -1;
}
