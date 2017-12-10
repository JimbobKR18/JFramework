#ifndef __JFramework__SDLSoundSystem__
#define __JFramework__SDLSoundSystem__

#include "SoundSystem.h" // Base class: SoundSystem

#if defined(_WIN32)
#include "SDL.h"
#include "SDL_mixer.h"
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

class SDLSoundSystem : public SoundSystem
{
private:
  static int const MAX_VOLUME = 128;
  typedef std::unordered_map<int, Mix_Chunk*>       SDLSoundContainer;
  typedef SDLSoundContainer::iterator               SDLSoundIt;
  typedef std::unordered_map<int, HashString>       PlayingSoundContainer;
  typedef PlayingSoundContainer::iterator           PlayingSoundIt;
  
  SDLSoundContainer mSoundContainer;
  PlayingSoundContainer mPlayingSounds;

public:
  SDLSoundSystem();
  virtual ~SDLSoundSystem();

  virtual void Update(float const aDT);
  virtual void CreateSound(HashString const& aFilename, HashString const& aAlias = "");
  virtual void DeleteSound(HashString const& aName);
  virtual int PlaySound(HashString const& aName, int const aNumLoops);
  virtual int PlaySoundTimed(HashString const& aName, int const aNumLoops, int const aMillis);
  virtual int FadeInSound(HashString const& aName, int const aNumLoops, int const aTime);
  virtual int FadeInSoundTimed(HashString const& aName, int const aNumLoops, int const aFadeTime, int const aPlayTime);
  virtual void ResumeSound(int const aChannel);
  virtual void PauseSound(int const aChannel);
  virtual void SetVolume(float const aVolume);
  virtual void SetChannelVolume(int const aChannel, float const aVolume);
  virtual void StopSound(int const aChannel);
  virtual void StopSoundTimed(int const aChannel, int const aMillis);
};

#endif // __JFramework__SDLSoundSystem__
