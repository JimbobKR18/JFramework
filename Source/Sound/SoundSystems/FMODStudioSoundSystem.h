#ifndef FMODSTUDIOSOUNDSYSTEM_H
#define FMODSTUDIOSOUNDSYSTEM_H

#include "SoundSystem.h" // Base class: SoundSystem
#include "fmod_studio.hpp"
#include "fmod_errors.h"

class FMODStudioSoundSystem : public SoundSystem
{
private:
  typedef std::unordered_map<int, FMOD::Studio::Bank*> BankContainer;
  typedef BankContainer::iterator BankIt;
  typedef BankContainer::const_iterator BankConstIt;
  typedef std::unordered_map<int, FMOD::Studio::EventDescription*> EventDescriptionContainer;
  typedef EventDescriptionContainer::iterator EventDescriptionIt;
  typedef EventDescriptionContainer::const_iterator EventDescriptionConstIt;
  typedef std::unordered_map<int, FMOD::Studio::EventInstance*> EventInstanceContainer;
  typedef EventInstanceContainer::iterator EventInstanceIt;
  typedef EventInstanceContainer::const_iterator EventInstanceConstIt;
  typedef std::unordered_set<int> InstanceContainer;
  typedef InstanceContainer::iterator InstanceIt;
  typedef InstanceContainer::const_iterator InstanceConstIt;
  typedef std::unordered_map<int, InstanceContainer> InstanceGroupContainer;
  typedef InstanceGroupContainer::iterator InstanceGroupIt;
  typedef InstanceGroupContainer::const_iterator InstanceGroupConstIt;
  
  FMOD::Studio::System* mFMODStudioSystem;
  BankContainer mBanks;
  EventDescriptionContainer mEventDescriptions;
  EventInstanceContainer mEventInstances;
  InstanceGroupContainer mGroups;
  int mCurrentSound;
  
public:
  FMODStudioSoundSystem();
  virtual ~FMODStudioSoundSystem();
  
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

#endif // FMODSTUDIOSOUNDSYSTEM_H
