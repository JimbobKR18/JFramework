#ifndef __JFramework_SoundSystem_h_
#define __JFramework_SoundSystem_h_

#include "Common.h"
#include "DSP_Type.h"
#include "DSP.h"

class SoundSystem
{
public:
  enum SoundSource
  {
    DEFAULT = 0,
    STREAM
  };

  SoundSystem();
  virtual ~SoundSystem();
  
  // Update
  virtual void Update(float const aDT) = 0;
  
  // Sound init and play init
  virtual void CreateSound(HashString const& aFilename, HashString const& aAlias, SoundSource const& aSource) = 0;
  virtual void DeleteSound(HashString const& aName) = 0;
  virtual int PlaySound(HashString const& aName, int const aNumLoops) = 0;
  
  // Channels
  virtual void ResumeChannel(int const aChannel) = 0;
  virtual void PauseChannel(int const aChannel) = 0;
  virtual void SetMasterVolume(float const aVolume) = 0;
  virtual void SetChannelVolume(int const aChannel, float const aVolume) = 0;
  virtual void StopChannel(int const aChannel) = 0;
  virtual void SetChannelFrequency(int const aChannel, float const aFrequency) = 0;
  
  // Channel groups
  virtual void CreateChannelGroup(HashString const &aGroupName) = 0;
  virtual void AddChannelToGroup(HashString const &aGroupName, int const aChannel) = 0;
  virtual void SetChannelGroupVolume(HashString const &aGroupName, float const aVolume) = 0;
  virtual void StopChannelGroup(HashString const &aGroupName) = 0;
  
  // DSPs
  virtual DSP* CreateDSP(HashString const &aName, DSP_Type const &aType) = 0;
  virtual void DeleteDSP(DSP *aDSP) = 0;
  virtual void AddDSPToChannel(DSP *aDSP, int const aChannel, int const aIndex) = 0;
  virtual void AddDSPToChannelGroup(DSP *aDSP, HashString const& aGroupName, int const aIndex) = 0;
  virtual void RemoveDSPFromChannel(DSP *aDSP, int const aChannel) = 0;
  virtual void RemoveDSPFromChannelGroup(DSP *aDSP, HashString const& aGroupName) = 0;
};

#endif // __JFramework_SoundSystem_h_
