/*
 * SoundManager.cpp
 * Jimmy Spencer
 * 1/27/13
 *
 * The idea: A general manager for sounds, what else could it be?
 *           Will be using a std::map I think, makes sense.
 */

#ifndef __JFramework_SoundManager_h_
#define __JFramework_SoundManager_h_

#include "Common.h"
#include "Manager.h"
#include "SoundSystem.h"
#include "DSP.h"

class SoundManager : public Manager
{
private:
  struct SoundDSPInfo
  {
    int mIndex;
    HashString mDSPName;
    
    SoundDSPInfo(int const aIndex, HashString const &aDSPName) : mIndex(aIndex), mDSPName(aDSPName)
    {}
    virtual ~SoundDSPInfo() {}
  };

  typedef std::set<HashString>                          SoundNameContainer;
  typedef std::vector<SoundDSPInfo*>                    SoundDSPInfoVector;
  typedef std::unordered_map<int, DSP*>                 DSPContainer;
  typedef std::unordered_map<int, SoundDSPInfoVector>   SoundDSPContainer;
  typedef std::unordered_map<int, SoundNameContainer>   SoundGroupContainer;
  typedef std::unordered_map<int, HashString>           AliasContainer;
  typedef DSPContainer::iterator                        DSPIt;
  typedef SoundDSPInfoVector::iterator                  SoundDSPInfoVectorIt;
  typedef SoundDSPContainer::iterator                   SoundDSPContainerIt;
  typedef SoundNameContainer::iterator                  SoundNameContainerIt;
  typedef SoundGroupContainer::iterator                 SoundGroupContainerIt;
  typedef AliasContainer::iterator                      AliasContainerIt;

  SoundSystem* mSoundSystem;
  DSPContainer mDSPContainer;
  SoundDSPContainer mSoundsToDSPs;
  SoundGroupContainer mSoundGroups;
  SoundDSPContainer mGroupsToDSPs;
  AliasContainer mAliases;

  static unsigned const sUID;

public:
  static int const INFINITE_LOOPS = -1;
  static int const ONE_LOOP = 0;

  SoundManager(GameApp *aApp);
  ~SoundManager();
  
  // Sound init and play init
  void LoadSoundBank(HashString const& aFilename, SoundSystem::SoundSource const &aSource = SoundSystem::SoundSource::DEFAULT);
  void UnloadSoundBank(HashString const& aFilename);
  void CreateSound(HashString const& aFilename, HashString const &aAlias = "", float const &aDefaultVolume = 1.0f,
                   SoundSystem::SoundSource const &aSource = SoundSystem::SoundSource::DEFAULT);
  void DeleteSound(HashString const& aName);
  int PlaySound(HashString const& aName, int const aNumLoops);
  
  // Channels
  void ResumeChannel(int const aChannel);
  void PauseChannel(int const aChannel);
  void SetMasterVolume(float const aVolume);
  void StopAllChannels();
  void SetChannelVolume(int const aChannel, float const aVolume);
  void StopChannel(int const aChannel);
  void FadeChannel(int const aChannel, int const aTime, float const aStart, float const aEnd);
  void DelayChannel(int const aChannel, int const aStartDelay, int const aEndDelay, bool const aStopChannels);
  float GetChannelProperty(int const aChannel, HashString const &aName);
  void SetChannelProperty(int const aChannel, HashString const &aName, float const aValue);
  void SetChannelFrequency(int const aChannel, float const aFrequency);
  void SetChannel3DAttributes(int const aChannel, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos);
  void SetChannel3DCone(int const aChannel, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume);
  void SetChannel3DAttenuation(int const aChannel, std::vector<Vector3> const &aPoints);
  void SetChannel3DMinMaxDistance(int const aChannel, float const aMinDistance, float const aMaxDistance);
  void SetChannel3DOcclusion(int const aChannel, float const aDirectOcclusion, float const aReverbOcclusion);
  void SetChannel3DSpread(int const aChannel, float const aAngle);
  
  // Channel groups
  bool HasChannelGroup(HashString const &aGroupName);
  void CreateChannelGroup(HashString const &aGroupName);
  void PlayChannelGroup(HashString const &aGroupName, int const aNumLoops);
  void AddSoundNameToGroup(HashString const &aGroupName, HashString const &aSoundName);
  void AddChannelToGroup(HashString const &aGroupName, int const aChannel);
  void RemoveChannelFromGroups(int const aChannel);
  void SetChannelGroupVolume(HashString const &aGroupName, float const aVolume);
  void StopChannelGroup(HashString const &aGroupName);
  void FadeChannelGroup(HashString const &aGroupName, int const aTime, float const aStart, float const aEnd);
  void SetChannelGroup3DAttributes(HashString const &aGroupName, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos);
  void SetChannelGroup3DCone(HashString const &aGroupName, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume);
  void SetChannelGroup3DAttenuation(HashString const &aGroupName, std::vector<Vector3> const &aPoints);
  void SetChannelGroup3DMinMaxDistance(HashString const &aGroupName, float const aMinDistance, float const aMaxDistance);
  void SetChannelGroup3DOcclusion(HashString const &aGroupName, float const aDirectOcclusion, float const aReverbOcclusion);
  void SetChannelGroup3DSpread(HashString const &aGroupName, float const aAngle);
  
  // DSPs
  bool HasDSP(HashString const &aName);
  DSP* CreateDSP(HashString const &aName, DSP_Type const &aType);
  DSP* GetDSPFromChannel(int aChannel, int aIndex);
  DSP* GetDSPFromChannelGroup(HashString const &aGroupName, int aIndex);
  void DeleteDSP(DSP *aDSP);
  void AddDSPToChannel(DSP *aDSP, int const aChannel, int const aIndex);
  void AddDSPToChannelGroup(DSP *aDSP, HashString const& aGroupName, int const aIndex);
  void RemoveDSPFromChannel(DSP *aDSP, int const aChannel);
  void RemoveDSPFromChannelGroup(DSP *aDSP, HashString const& aGroupName);
  
  // Buses
  void SetBusMuteState(HashString const &aBusName, bool const aMute);
  void SetBusPauseState(HashString const &aBusName, bool const aPause);
  void SetBusVolume(HashString const &aBusName, float const aVolume);
  void StopBus(HashString const &aBusName);
  void FadeBus(HashString const &aBusName);
  void LockBus(HashString const &aBusName);
  void UnlockBus(HashString const &aBusName);

  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "SoundManager";}
  static unsigned     GetUID() {return sUID;}

  static void         SerializeLUA();
  
private:
  HashString          GetAliasIfAvailable(HashString const &aName);
  void                LoadSounds();
};

#endif
