#include "SDLSoundSystem.h"

SDLSoundSystem::SDLSoundSystem() : mSoundContainer(), mPlayingSounds()
{
  int flags = MIX_INIT_OGG;
  int initted = Mix_Init(flags);
  if((initted & flags) != flags)
  {
    DebugLogPrint("Mix_Init: %s\n", Mix_GetError());
  }
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
  {
    DebugLogPrint("Mix_OpenAudio: %s\n", Mix_GetError());
    assert(!"Could not open SDL_Mix OpenAudio, aborting.");
  }
}

SDLSoundSystem::~SDLSoundSystem()
{
  for(SDLSoundIt it = mSoundContainer.begin(); it != mSoundContainer.end(); ++it)
  {
    Mix_FreeChunk(it->second);
  }
  mSoundContainer.clear();
  
  Mix_Quit();
  Mix_CloseAudio();
}

void SDLSoundSystem::Update(float const aDT)
{
  for(PlayingSoundIt it = mPlayingSounds.begin(); it != mPlayingSounds.end();)
  {
    if(!Mix_Playing(it->first))
    {
      it = mPlayingSounds.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

void SDLSoundSystem::CreateSound(HashString const& aFilename, HashString const& aAlias)
{
  HashString fileName = Common::RelativePath("Sounds", aFilename);
  Mix_Chunk* chunk = Mix_LoadWAV(fileName.ToCharArray());
  if(!chunk)
  {
    DebugLogPrint("Mix_QuickLoad_WAV: %s\n%s\n", Mix_GetError(), aFilename.ToCharArray());
#if defined(_DEBUG) && defined(_SOUND_DEBUG)
    assert(!"Mix_Quickload_WAV failed, aborting.");
#endif
  }
  
  if(!aAlias.Empty())
  {
    mSoundContainer[aAlias.ToHash()] = chunk;
  }
  else
  {
    mSoundContainer[aFilename.ToHash()] = chunk;
  }
}

void SDLSoundSystem::DeleteSound(HashString const& aName)
{
  Mix_FreeChunk(mSoundContainer[aName.ToHash()]);
  mSoundContainer.erase(aName.ToHash());
}

int SDLSoundSystem::PlaySound(HashString const& aName, int const aNumLoops)
{
  if(mSoundContainer.find(aName.ToHash()) == mSoundContainer.end())
    assert(!"Sound not loaded.");
  
  int channel = Mix_PlayChannel(-1, mSoundContainer[aName.ToHash()], aNumLoops);
  if(channel == -1)
  {
    DebugLogPrint("Mix_PlayChannel: %s\n", Mix_GetError());
#if defined(_DEBUG) && defined(_SOUND_DEBUG)
    assert(!"Mix_PlayChannel failed, aborting.");
#endif
  }
  mPlayingSounds[channel] = aName;
  return channel;
}

int SDLSoundSystem::PlaySoundTimed(HashString const& aName, int const aNumLoops, int const aMillis)
{
  if(mSoundContainer.find(aName.ToHash()) == mSoundContainer.end())
    assert(!"Sound not loaded.");
  
  int channel = Mix_PlayChannelTimed(-1, mSoundContainer[aName.ToHash()], aNumLoops, aMillis);
  if(channel == -1)
  {
    DebugLogPrint("Mix_PlayChannelTimed: %s\n", Mix_GetError());
#if defined(_DEBUG) && defined(_SOUND_DEBUG)
    assert(!"Mix_PlayChannelTimed failed, aborting.");
#endif
  }
  mPlayingSounds[channel] = aName;
  return channel;
}

int SDLSoundSystem::FadeInSound(HashString const& aName, int const aNumLoops, int const aTime)
{
  if(mSoundContainer.find(aName.ToHash()) == mSoundContainer.end())
    assert(!"Sound not loaded.");
  
  int channel = Mix_FadeInChannel(-1, mSoundContainer[aName.ToHash()], aNumLoops, aTime);
  if(channel == -1)
  {
    DebugLogPrint("Mix_FadeInChannel: %s\n", Mix_GetError());
#if defined(_DEBUG) && defined(_SOUND_DEBUG)
    assert(!"Mix_FadeInChannel failed, aborting.");
#endif
  }
  return channel;
}

int SDLSoundSystem::FadeInSoundTimed(HashString const& aName, int const aNumLoops, int const aFadeTime, int const aPlayTime)
{
  if(mSoundContainer.find(aName.ToHash()) == mSoundContainer.end())
    assert(!"Sound not loaded.");
  
  int channel = Mix_FadeInChannelTimed(-1, mSoundContainer[aName.ToHash()], aNumLoops, aFadeTime, aPlayTime);
  if(channel == -1)
  {
    DebugLogPrint("Mix_FadeInChannelTimed: %s\n", Mix_GetError());
#if defined(_DEBUG) && defined(_SOUND_DEBUG)
    assert(!"Mix_FadeInChannelTimed failed, aborting.");
#endif
  }
  return channel;
}

void SDLSoundSystem::ResumeSound(int const aChannel)
{
  Mix_Resume(aChannel);
}

void SDLSoundSystem::SetVolume(float const aVolume)
{
  for(PlayingSoundIt it = mPlayingSounds.begin(); it != mPlayingSounds.end();)
  {
    Mix_Volume(it->first, aVolume * MAX_VOLUME);
  }
}

void SDLSoundSystem::SetChannelVolume(int const aChannel, float const aVolume)
{
  Mix_Volume(aChannel, aVolume * MAX_VOLUME);
}

void SDLSoundSystem::PauseSound(int const aChannel)
{
  Mix_Pause(aChannel);
}

void SDLSoundSystem::StopSound(int const aChannel)
{
  Mix_HaltChannel(aChannel);
}

void SDLSoundSystem::StopSoundTimed(int const aChannel, int const aMillis)
{
  Mix_FadeOutChannel(aChannel, aMillis);
}
