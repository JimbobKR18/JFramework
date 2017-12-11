#ifndef __JFramework_SoundSystem_h_
#define __JFramework_SoundSystem_h_

#include "Common.h"

class SoundSystem
{
public:
  SoundSystem();
  virtual ~SoundSystem();
  
  virtual void Update(float const aDT) = 0;
  virtual void CreateSound(HashString const& aFilename, HashString const& aAlias) = 0;
  virtual void DeleteSound(HashString const& aName) = 0;
  virtual int PlaySound(HashString const& aName, int const aNumLoops) = 0;
  virtual int FadeInSound(HashString const& aName, int const aNumLoops, int const aTime) = 0;
  virtual void ResumeSound(int const aChannel) = 0;
  virtual void PauseSound(int const aChannel) = 0;
  virtual void SetVolume(float const aVolume) = 0;
  virtual void SetChannelVolume(int const aChannel, float const aVolume) = 0;
  virtual void StopSound(int const aChannel) = 0;
  virtual void FadeOutSound(int const aChannel, int const aMillis) = 0;
};

#endif // __JFramework_SoundSystem_h_
