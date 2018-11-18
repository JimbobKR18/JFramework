#ifndef __JFramework__FMODSoundSystem_h_
#define __JFramework__FMODSoundSystem_h_

#include "SoundSystem.h" // Base class: SoundSystem
#include "fmod_studio.hpp"
#include "fmod_errors.h"

#define FMOD_SAMPLES_PER_SECOND 44100

struct FMODSound
{
  FMOD::Sound* mSound;
  float mVolume;
  
  FMODSound(FMOD::Sound* aSound, float const &aVolume);
  virtual ~FMODSound();
};

class FMODSoundSystem : public SoundSystem
{
private:
  typedef std::unordered_map<int, FMODSound*>     FMODSoundContainer;
  typedef FMODSoundContainer::iterator              FMODSoundIt;
  typedef std::unordered_map<int, FMOD::ChannelGroup*> FMODChannelGroupContainer;
  typedef FMODChannelGroupContainer::iterator          FMODChannelGroupIt;
  typedef std::unordered_map<int, FMOD::DSP*>     FMODDSPContainer;
  typedef FMODDSPContainer::iterator              FMODDSPIt;

  FMOD::Studio::System* mFMODStudioSystem;
  FMOD::System* mFMODSystem;
  FMOD::ChannelGroup* mMasterChannelGroup;
  FMODSoundContainer mSoundContainer;
  FMODChannelGroupContainer mChannelGroupContainer;
  FMODDSPContainer mDSPContainer;
  
public:
  FMODSoundSystem();
  virtual ~FMODSoundSystem();
  
  // Getters
  FMOD::DSP* GetDSP(HashString const &aName);

  // Update
  virtual void Update(float const aDT);
  
  // Sound init and play init
  virtual void LoadSoundBank(HashString const& aFilename, SoundSource const& aSource);
  virtual void UnloadSoundBank(HashString const& aFilename);
  virtual void CreateSound(HashString const& aFilename, float const &aDefaultVolume, SoundSource const& aSource);
  virtual void DeleteSound(HashString const& aName);
  virtual int PlaySound(HashString const& aName, int const aNumLoops);
  
  // Channels
  virtual void ResumeChannel(int const aChannel);
  virtual void PauseChannel(int const aChannel);
  virtual void SetMasterVolume(float const aVolume);
  virtual void StopAllChannels();
  virtual void SetChannelVolume(int const aChannel, float const aVolume);
  virtual void StopChannel(int const aChannel);
  virtual void FadeChannel(int const aChannel, int const aTime, float const aStart, float const aEnd);
  virtual void DelayChannel(int const aChannel, int const aStartDelay, int const aEndDelay, bool const aStopChannels);
  virtual float GetChannelProperty(int const aChannel, HashString const &aName);
  virtual void SetChannelProperty(int const aChannel, HashString const &aName, float const aValue);
  virtual void SetChannelFrequency(int const aChannel, float const aFrequency);
  virtual void SetChannel3DAttributes(int const aChannel, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos);
  virtual void SetChannel3DCone(int const aChannel, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume);
  virtual void SetChannel3DAttenuation(int const aChannel, std::vector<Vector3> const &aPoints);
  virtual void SetChannel3DMinMaxDistance(int const aChannel, float const aMinDistance, float const aMaxDistance);
  virtual void SetChannel3DOcclusion(int const aChannel, float const aDirectOcclusion, float const aReverbOcclusion);
  virtual void SetChannel3DSpread(int const aChannel, float const aAngle);
  
  // Channel groups
  virtual void CreateChannelGroup(HashString const &aGroupName);
  virtual void AddChannelToGroup(HashString const &aGroupName, int const aChannel);
  virtual void RemoveChannelFromGroups(int const aChannel);
  virtual void SetChannelGroupVolume(HashString const &aGroupName, float const aVolume);
  virtual void StopChannelGroup(HashString const &aGroupName);
  virtual void FadeChannelGroup(HashString const &aGroupName, int const aTime, float const aStart, float const aEnd);
  virtual void SetChannelGroup3DAttributes(HashString const &aGroupName, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos);
  virtual void SetChannelGroup3DCone(HashString const &aGroupName, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume);
  virtual void SetChannelGroup3DAttenuation(HashString const &aGroupName, std::vector<Vector3> const &aPoints);
  virtual void SetChannelGroup3DMinMaxDistance(HashString const &aGroupName, float const aMinDistance, float const aMaxDistance);
  virtual void SetChannelGroup3DOcclusion(HashString const &aGroupName, float const aDirectOcclusion, float const aReverbOcclusion);
  virtual void SetChannelGroup3DSpread(HashString const &aGroupName, float const aAngle);
  
  // DSPs
  virtual DSP* CreateDSP(HashString const &aName, DSP_Type const &aType);
  virtual DSP* GetDSPFromChannel(int aChannel, int aIndex);
  virtual DSP* GetDSPFromChannelGroup(HashString const &aGroupName, int aIndex);
  virtual bool DeleteDSP(DSP *aDSP);
  virtual void AddDSPToChannel(DSP *aDSP, int const aChannel, int const aIndex);
  virtual void AddDSPToChannelGroup(DSP *aDSP, HashString const& aGroupName, int const aIndex);
  virtual void RemoveDSPFromChannel(DSP *aDSP, int const aChannel);
  virtual void RemoveDSPFromChannelGroup(DSP *aDSP, HashString const& aGroupName);
  
  // Buses
  virtual void SetBusMuteState(HashString const &aBusName, bool const aMute);
  virtual void SetBusPauseState(HashString const &aBusName, bool const aPause);
  virtual void SetBusVolume(HashString const &aBusName, float const aVolume);
  virtual void StopBus(HashString const &aBusName);
  virtual void FadeBus(HashString const &aBusName);
  virtual void LockBus(HashString const &aBusName);
  virtual void UnlockBus(HashString const &aBusName);
};

#endif // __JFramework__FMODSoundSystem_h_
