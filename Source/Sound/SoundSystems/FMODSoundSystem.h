#ifndef __JFramework__FMODSoundSystem__
#define __JFramework__FMODSoundSystem__

#include "SoundSystem.h" // Base class: SoundSystem
#include "fmod_studio.hpp"
#include "fmod_errors.h"

class FMODSoundSystem : public SoundSystem
{
private:
  typedef std::unordered_map<int, FMOD::Sound*>     FMODSoundContainer;
  typedef FMODSoundContainer::iterator              FMODSoundIt;

  FMOD::Studio::System* mFMODStudioSystem;
  FMOD::System* mFMODSystem;
  FMOD::ChannelGroup* mMasterChannelGroup;
  FMODSoundContainer mSoundContainer;
  
public:
  FMODSoundSystem();
  virtual ~FMODSoundSystem();

  virtual void CreateSound(HashString const& aFilename, HashString const& aAlias = "");
  virtual void DeleteSound(HashString const& aName);
  virtual int FadeInSound(HashString const& aName, int const aNumLoops, int const aTime);
  virtual void PauseSound(int const aChannel);
  virtual int PlaySound(HashString const& aName, int const aNumLoops);
  virtual void ResumeSound(int const aChannel);
  virtual void SetChannelVolume(int const aChannel, float const aVolume);
  virtual void SetVolume(float const aVolume);
  virtual void StopSound(int const aChannel);
  virtual void FadeOutSound(int const aChannel, int const aMillis);
  virtual void Update(float const aDT);
};

#endif // __JFramework__FMODSoundSystem__
