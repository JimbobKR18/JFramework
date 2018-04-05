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
  typedef std::unordered_map<int, DSP*>     DSPContainer;
  typedef DSPContainer::iterator            DSPIt;

  SoundSystem* mSoundSystem;
  DSPContainer mDSPContainer;

  static unsigned const sUID;

public:
  static int const INFINITE_LOOPS = -1;
  static int const ONE_LOOP = 0;

  SoundManager(GameApp *aApp);
  ~SoundManager();
  
  // Sound init and play init
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
  void SetChannelFrequency(int const aChannel, float const aFrequency);
  void SetChannel3DAttributes(int const aChannel, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos);
  void SetChannel3DCone(int const aChannel, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume);
  void SetChannel3DAttenuation(int const aChannel, std::vector<Vector3> const &aPoints);
  void SetChannel3DMinMaxDistance(int const aChannel, float const aMinDistance, float const aMaxDistance);
  void SetChannel3DOcclusion(int const aChannel, float const aDirectOcclusion, float const aReverbOcclusion);
  void SetChannel3DSpread(int const aChannel, float const aAngle);
  
  // Channel groups
  void CreateChannelGroup(HashString const &aGroupName);
  void AddChannelToGroup(HashString const &aGroupName, int const aChannel);
  void SetChannelGroupVolume(HashString const &aGroupName, float const aVolume);
  void StopChannelGroup(HashString const &aGroupName);
  void SetChannelGroup3DAttributes(HashString const &aGroupName, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos);
  void SetChannelGroup3DCone(HashString const &aGroupName, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume);
  void SetChannelGroup3DAttenuation(HashString const &aGroupName, std::vector<Vector3> const &aPoints);
  void SetChannelGroup3DMinMaxDistance(HashString const &aGroupName, float const aMinDistance, float const aMaxDistance);
  void SetChannelGroup3DOcclusion(HashString const &aGroupName, float const aDirectOcclusion, float const aReverbOcclusion);
  void SetChannelGroup3DSpread(HashString const &aGroupName, float const aAngle);
  
  // DSPs
  DSP* CreateDSP(HashString const &aName, DSP_Type const &aType);
  DSP* GetDSPFromChannel(int aChannel, int aIndex);
  DSP* GetDSPFromChannelGroup(HashString const &aGroupName, int aIndex);
  void DeleteDSP(DSP *aDSP);
  void AddDSPToChannel(DSP *aDSP, int const aChannel, int const aIndex);
  void AddDSPToChannelGroup(DSP *aDSP, HashString const& aGroupName, int const aIndex);
  void RemoveDSPFromChannel(DSP *aDSP, int const aChannel);
  void RemoveDSPFromChannelGroup(DSP *aDSP, HashString const& aGroupName);

  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "SoundManager";}
  static unsigned     GetUID() {return sUID;}

  static void         SerializeLUA();
  
private:
  void                LoadSounds();
};

#endif
