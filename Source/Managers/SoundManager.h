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
  SoundSystem*                                      mSoundSystem;

  static unsigned const sUID;

public:
  static int const INFINITE_LOOPS = -1;
  static int const ONE_LOOP = 0;

  SoundManager(GameApp *aApp);
  ~SoundManager();
  
  // Sound init and play init
  void CreateSound(HashString const& aFilename, HashString const &aAlias = "", 
                   SoundSystem::SoundSource const &aSource = SoundSystem::SoundSource::DEFAULT);
  void DeleteSound(HashString const& aName);
  int PlaySound(HashString const& aName, int const aNumLoops);
  
  // Channels
  void ResumeChannel(int const aChannel);
  void PauseChannel(int const aChannel);
  void SetMasterVolume(float const aVolume);
  void SetChannelVolume(int const aChannel, float const aVolume);
  void StopChannel(int const aChannel);
  void SetChannelFrequency(int const aChannel, float const aFrequency);
  
  // Channel groups
  void CreateChannelGroup(HashString const &aGroupName);
  void AddChannelToGroup(HashString const &aGroupName, int const aChannel);
  void SetChannelGroupVolume(HashString const &aGroupName, float const aVolume);
  void StopChannelGroup(HashString const &aGroupName);
  
  // DSPs
  DSP* CreateDSP(HashString const &aName, DSP_Type const &aType);
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
