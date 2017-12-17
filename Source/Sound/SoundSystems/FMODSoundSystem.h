#ifndef __JFramework__FMODSoundSystem_h_
#define __JFramework__FMODSoundSystem_h_

#include "SoundSystem.h" // Base class: SoundSystem
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include "FMODDSP.h"

class FMODSoundSystem : public SoundSystem
{
private:
  typedef std::unordered_map<int, FMOD::Sound*>     FMODSoundContainer;
  typedef FMODSoundContainer::iterator              FMODSoundIt;
  typedef std::unordered_map<int, FMOD::ChannelGroup*> FMODChannelGroupContainer;
  typedef FMODChannelGroupContainer::iterator          FMODChannelGroupIt;
  typedef std::unordered_map<int, FMODDSP*>     FMODDSPContainer;
  typedef FMODDSPContainer::iterator            FMODDSPIt;

  FMOD::Studio::System* mFMODStudioSystem;
  FMOD::System* mFMODSystem;
  FMOD::ChannelGroup* mMasterChannelGroup;
  FMODSoundContainer mSoundContainer;
  FMODChannelGroupContainer mChannelGroupContainer;
  FMODDSPContainer mDSPContainer;
  
public:
  FMODSoundSystem();
  virtual ~FMODSoundSystem();

  // Update
  virtual void Update(float const aDT);
  
  // Sound init and play init
  virtual void CreateSound(HashString const& aFilename, HashString const& aAlias, SoundSource const& aSource);
  virtual void DeleteSound(HashString const& aName);
  virtual int PlaySound(HashString const& aName, int const aNumLoops);
  
  // Channels
  virtual void ResumeChannel(int const aChannel);
  virtual void PauseChannel(int const aChannel);
  virtual void SetMasterVolume(float const aVolume);
  virtual void SetChannelVolume(int const aChannel, float const aVolume);
  virtual void StopChannel(int const aChannel);
  virtual void SetChannelFrequency(int const aChannel, float const aFrequency);
  
  // Channel groups
  virtual void CreateChannelGroup(HashString const &aGroupName);
  virtual void AddChannelToGroup(HashString const &aGroupName, int const aChannel);
  virtual void SetChannelGroupVolume(HashString const &aGroupName, float const aVolume);
  virtual void StopChannelGroup(HashString const &aGroupName);
  
  // DSPs
  virtual DSP* CreateDSP(HashString const &aName, DSP_Type const &aType);
  virtual DSP* GetDSPFromChannel(int aChannel, int aIndex);
  virtual DSP* GetDSPFromChannelGroup(HashString const &aGroupName, int aIndex);
  virtual void DeleteDSP(DSP *aDSP);
  virtual void AddDSPToChannel(DSP *aDSP, int const aChannel, int const aIndex);
  virtual void AddDSPToChannelGroup(DSP *aDSP, HashString const& aGroupName, int const aIndex);
  virtual void RemoveDSPFromChannel(DSP *aDSP, int const aChannel);
  virtual void RemoveDSPFromChannelGroup(DSP *aDSP, HashString const& aGroupName);
};

#endif // __JFramework__FMODSoundSystem_h_
