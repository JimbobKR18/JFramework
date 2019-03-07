/* SoundManager.cpp
 * Jimmy Spencer
 * 1/27/13
 *
 * The idea: The implementation of SoundManager, what else?
 */


#include "SoundManager.h"
#include "Common.h"
#include "ParserFactory.h"
#include "SystemProperties.h"
#if !defined(ANDROID) && !defined(IOS)
#include "FMODSoundSystem.h"
#include "FMODStudioSoundSystem.h"
#endif

unsigned const SoundManager::sUID = Common::StringHashFunction("SoundManager");
SoundManager::SoundManager(GameApp *aApp) : Manager(aApp, "SoundManager", SoundManager::sUID), mSoundSystem(nullptr), 
  mDSPContainer(), mSoundsToDSPs(), mSoundGroups(), mAliases()
{
#if !defined(ANDROID) && !defined(IOS)
  if(SystemProperties::GetSoundEngine() == "FMOD_STUDIO")
    mSoundSystem = new FMODStudioSoundSystem();
  else
    mSoundSystem = new FMODSoundSystem();
#else
  assert(!"Platform not supported. (SoundManager.cpp)");
#endif

  LoadSounds();
  mSoundSystem->SetMasterVolume(SystemProperties::GetDefaultVolume());
}

SoundManager::~SoundManager()
{
  delete mSoundSystem;
  
  // Erase all occurrences of sound to dsp linkage.
  for(SoundDSPContainerIt it = mSoundsToDSPs.begin(); it != mSoundsToDSPs.end(); ++it)
  {
    SoundDSPInfoVector dsps = it->second;
    for(SoundDSPInfoVectorIt it2 = dsps.begin(); it2 != dsps.end(); ++it2)
    {
      delete *it2;
    }
    dsps.clear();
  }
  mSoundsToDSPs.clear();
  
  for(DSPIt it = mDSPContainer.begin(); it != mDSPContainer.end(); ++it)
  {
    delete it->second;
  }
  mDSPContainer.clear();
}

/**
 * @brief Load sound bank.
 * @param aFilename
 * @param aSource
 */
void SoundManager::LoadSoundBank(HashString const& aFilename, SoundSystem::SoundSource const& aSource)
{
  mSoundSystem->LoadSoundBank(aFilename, aSource);
}

/**
 * @brief Unload sound bank.
 * @param aFilename
 */
void SoundManager::UnloadSoundBank(HashString const& aFilename)
{
  mSoundSystem->UnloadSoundBank(aFilename);
}

/**
 * @brief Create a sound by filename.
 * @param aFilename
 * @param aAlias
 * @param aDefaultVolume
 * @param aSource
 */
void SoundManager::CreateSound(HashString const &aFilename, HashString const &aAlias, float const &aDefaultVolume, SoundSystem::SoundSource const &aSource)
{
  mSoundSystem->CreateSound(aFilename, aDefaultVolume, aSource);
  if(!aAlias.Empty())
  {
    mAliases[aAlias.ToHash()] = aFilename;
  }
}

/**
 * @brief Delete a sound.
 * @param aName
 */
void SoundManager::DeleteSound(HashString const &aName)
{
  HashString name = GetAliasIfAvailable(aName);
  mSoundSystem->DeleteSound(name);
  
  // Erase all occurrences of sound to dsp linkage.
  if(mSoundsToDSPs.find(aName.ToHash()) != mSoundsToDSPs.end())
  {
    SoundDSPInfoVector dsps = mSoundsToDSPs[aName.ToHash()];
    for(SoundDSPInfoVectorIt it = dsps.begin(); it != dsps.end(); ++it)
    {
      delete *it;
    }
    dsps.clear();
    mSoundsToDSPs.erase(aName.ToHash());
  }
  
  // Erase all occurrences in sound groups.
  for(SoundGroupContainerIt it = mSoundGroups.begin(); it != mSoundGroups.end(); ++it)
  {
    it->second.erase(name);
  }
}

/**
 * @brief Play a sound until it finishes.
 * @param aName
 * @param aNumLoops
 * @return Channel of sound
 */
int SoundManager::PlaySound(HashString const &aName, int const aNumLoops)
{
  HashString name = GetAliasIfAvailable(aName);
  int channel = mSoundSystem->PlaySound(name, aNumLoops);
  if(mSoundsToDSPs.find(aName.ToHash()) != mSoundsToDSPs.end())
  {
    SoundDSPInfoVector dsps = mSoundsToDSPs[aName.ToHash()];
    for(SoundDSPInfoVectorIt it = dsps.begin(); it != dsps.end(); ++it)
    {
      SoundDSPInfo* info = *it;
      if(mDSPContainer.find(info->mDSPName.ToHash()) != mDSPContainer.end())
      {
        DSP *dsp = mDSPContainer[info->mDSPName.ToHash()];
        AddDSPToChannel(dsp, channel, info->mIndex);
      }
    }
  }
  return channel;
}

/**
 * @brief Check if channel is paused
 * @param aChannel
 * @return True if paused
 */
bool SoundManager::IsChannelPaused(int const aChannel)
{
  return mSoundSystem->IsChannelPaused(aChannel);
}

/**
 * @brief Check if channel is playing
 * @param aChannel
 * @return True if playing
 */
bool SoundManager::IsChannelPlaying(int const aChannel)
{
  return mSoundSystem->IsChannelPlaying(aChannel);
}

/**
 * @brief Resume a sound from pause.
 * @param aChannel
 */
void SoundManager::ResumeChannel(int aChannel)
{
  mSoundSystem->ResumeChannel(aChannel);
}

/**
 * @brief Pause a sound.
 * @param aChannel
 */
void SoundManager::PauseChannel(int aChannel)
{
  mSoundSystem->PauseChannel(aChannel);
}

/**
 * @brief Set volume of all sounds.
 * @param aVolume Volume value from 0 to 1.
 */
void SoundManager::SetMasterVolume(float const aVolume)
{
  mSoundSystem->SetMasterVolume(aVolume);
}

/**
 * @brief Stop all sounds.
 */
void SoundManager::StopAllChannels()
{
  mSoundSystem->StopAllChannels();
}

/**
 * @brief Set the volume of a channel.
 * @param aChannel
 * @param aVolume The volume to set the sound to.
 */
void SoundManager::SetChannelVolume(int aChannel, float const aVolume)
{
  mSoundSystem->SetChannelVolume(aChannel, aVolume);
}

/**
 * @brief Stop a sound.
 * @param aChannel
 */
void SoundManager::StopChannel(int aChannel)
{
  mSoundSystem->StopChannel(aChannel);
}

/**
 * @brief Fade channel from start to end.
 * @param aChannel Channel to fade.
 * @param aTime Time in samples.
 * @param aStart Starting volume.
 * @param aEnd Ending volume.
 */
void SoundManager::FadeChannel(int const aChannel, int const aTime, float const aStart, float const aEnd)
{
  mSoundSystem->FadeChannel(aChannel, aTime, aStart, aEnd);
}

/**
 * @brief Delay channel start and end.
 * @param aChannel Channel to delay.
 * @param aStartDelay Time in samples.
 * @param aEndDelay Time in samples.
 * @param aStopChannels True if channel should stop when sound data ends.
 */
void SoundManager::DelayChannel(int const aChannel, int const aStartDelay, int const aEndDelay, bool const aStopChannels)
{
  mSoundSystem->DelayChannel(aChannel, aStartDelay, aEndDelay, aStopChannels);
}

/**
 * @brief Get channel property value.
 * @param aChannel Channel id.
 * @param aName Name of property.
 * @return Property value.
 */
float SoundManager::GetChannelProperty(int const aChannel, HashString const &aName)
{
  return mSoundSystem->GetChannelProperty(aChannel, aName);
}

/**
 * @brief Set channel property value.
 * @param aChannel Channel id.
 * @param aName Name of property.
 * @param aValue Value of property.
 */
void SoundManager::SetChannelProperty(int const aChannel, HashString const &aName, float const aValue)
{
  mSoundSystem->SetChannelProperty(aChannel, aName, aValue);
}

/**
 * @brief Set speed of channel via frequency
 * @param aChannel
 * @param aFrequency
 */
void SoundManager::SetChannelFrequency(int const aChannel, float const aFrequency)
{
  mSoundSystem->SetChannelFrequency(aChannel, aFrequency);
}

/**
 * @brief Set 3D attributes for channel
 * @param aGroupName
 * @param aPos
 * @param aVel
 * @param aAltPanPos
 */
void SoundManager::SetChannel3DAttributes(int const aChannel, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos)
{
  mSoundSystem->SetChannel3DAttributes(aChannel, aPos, aVel, aAltPanPos);
}

/**
 * @brief Set cone for channel
 * @param aGroupName
 * @param aOrientation
 * @param aInsideAngle
 * @param aOutsideAngle
 * @param aOutsideVolume
 */
void SoundManager::SetChannel3DCone(int const aChannel, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume)
{
  mSoundSystem->SetChannel3DCone(aChannel, aOrientation, aInsideAngle, aOutsideAngle, aOutsideVolume);
}

/**
 * @brief Set 3D attenuation for channel
 * @param aGroupName
 * @param aPoints
 */
void SoundManager::SetChannel3DAttenuation(int const aChannel, std::vector<Vector3> const &aPoints)
{
  mSoundSystem->SetChannel3DAttenuation(aChannel, aPoints);
}

/**
 * @brief Set 3D min max distance for channel
 * @param aGroupName
 * @param aMinDistance
 * @param aMaxDistance
 */
void SoundManager::SetChannel3DMinMaxDistance(int const aChannel, float const aMinDistance, float const aMaxDistance)
{
  mSoundSystem->SetChannel3DMinMaxDistance(aChannel, aMinDistance, aMaxDistance);
}

/**
 * @brief Set occlusion for channel
 * @param aGroupName
 * @param aDirectOcclusion
 * @param aReverbOcclusion
 */
void SoundManager::SetChannel3DOcclusion(int const aChannel, float const aDirectOcclusion, float const aReverbOcclusion)
{
  mSoundSystem->SetChannel3DOcclusion(aChannel, aDirectOcclusion, aReverbOcclusion);
}

/**
 * @brief Set spread for channel
 * @param aGroupName
 * @param aAngle
 */
void SoundManager::SetChannel3DSpread(int const aChannel, float const aAngle)
{
  mSoundSystem->SetChannel3DSpread(aChannel, aAngle);
}

/**
 * @brief Check if channel group is playing
 * @param aGroupName Name of group
 * @return True if group is playing
 */
bool SoundManager::IsChannelGroupPlaying(HashString const &aGroupName)
{
  return mSoundSystem->IsChannelGroupPlaying(aGroupName);
}

/**
 * @brief Return true if channel group exists
 * @param aGroupName Name of group
 * @return True if group exists
 */
bool SoundManager::HasChannelGroup(HashString const &aGroupName)
{
  return mSoundSystem->HasChannelGroup(aGroupName);
}

/**
 * @brief Create new channel group
 * @param aGroupName
 */
void SoundManager::CreateChannelGroup(HashString const &aGroupName)
{
  mSoundSystem->CreateChannelGroup(aGroupName);
  mSoundGroups[aGroupName.ToHash()] = SoundNameContainer();
}

/**
 * @brief Play channel group, with DSPs and all.
 * @param aGroupName
 * @param aNumLoops
 */
void SoundManager::PlayChannelGroup(HashString const &aGroupName, int const aNumLoops)
{
  if(mSoundGroups.find(aGroupName.ToHash()) == mSoundGroups.end())
  {
    DebugLogPrint("Channel group %s not found!", aGroupName.ToCharArray());
    return;
  }
  
  // Play sounds
  for(SoundNameContainerIt it = mSoundGroups[aGroupName.ToHash()].begin(); it != mSoundGroups[aGroupName.ToHash()].end(); ++it)
  {
    int channel = PlaySound(*it, aNumLoops);
    AddChannelToGroup(aGroupName, channel);
  }
  
  // Add DSP to groups
  if(mGroupsToDSPs.find(aGroupName.ToHash()) != mGroupsToDSPs.end())
  {
    SoundDSPInfoVector dsps = mGroupsToDSPs[aGroupName.ToHash()];
    for(SoundDSPInfoVectorIt it = dsps.begin(); it != dsps.end(); ++it)
    {
      SoundDSPInfo* info = *it;
      if(mDSPContainer.find(info->mDSPName.ToHash()) != mDSPContainer.end())
      {
        DSP *dsp = mDSPContainer[info->mDSPName.ToHash()];
        AddDSPToChannelGroup(dsp, aGroupName, info->mIndex);
      }
    }
  }
}

/**
 * @brief Add sound to always be played when channel group plays.
 * @param aGroupName
 * @param aSoundName
 */
void SoundManager::AddSoundNameToGroup(HashString const &aGroupName, HashString const &aSoundName)
{
  if(mSoundGroups.find(aGroupName.ToHash()) == mSoundGroups.end())
    CreateChannelGroup(aGroupName);
  
  mSoundGroups[aGroupName.ToHash()].insert(aSoundName);
}

/**
 * @brief Add channel to group, this will not add sound to always play when you tell group to play. Consider it a temporary measure.
 * @param aGroupName
 * @param aChannel
 */
void SoundManager::AddChannelToGroup(HashString const &aGroupName, int const aChannel)
{
  mSoundSystem->AddChannelToGroup(aGroupName, aChannel);
}

/**
 * @brief Remove channel from groups
 * @param aChannel Channel to remove
 */
void SoundManager::RemoveChannelFromGroups(int const aChannel)
{
  mSoundSystem->RemoveChannelFromGroups(aChannel);
}

/**
 * @brief Set volume of channel group
 * @param aGroupName
 * @param aVolume
 */
void SoundManager::SetChannelGroupVolume(HashString const &aGroupName, float const aVolume)
{
  mSoundSystem->SetChannelGroupVolume(aGroupName, aVolume);
}

/**
 * @brief Stop playing a channel group
 * @param aGroupName
 */
void SoundManager::StopChannelGroup(HashString const &aGroupName)
{
  mSoundSystem->StopChannelGroup(aGroupName);
}

/**
 * @brief Fade channel group from start to end.
 * @param aGroupName Channel group to fade.
 * @param aTime Time in samples.
 * @param aStart Starting volume.
 * @param aEnd Ending volume.
 */
void SoundManager::FadeChannelGroup(HashString const &aGroupName, int const aTime, float const aStart, float const aEnd)
{
  mSoundSystem->FadeChannelGroup(aGroupName, aTime, aStart, aEnd);
}

/**
 * @brief Set 3D attributes for channel group
 * @param aGroupName
 * @param aPos
 * @param aVel
 * @param aAltPanPos
 */
void SoundManager::SetChannelGroup3DAttributes(HashString const &aGroupName, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos)
{
  mSoundSystem->SetChannelGroup3DAttributes(aGroupName, aPos, aVel, aAltPanPos);
}

/**
 * @brief Set cone for channel group
 * @param aGroupName
 * @param aOrientation
 * @param aInsideAngle
 * @param aOutsideAngle
 * @param aOutsideVolume
 */
void SoundManager::SetChannelGroup3DCone(HashString const &aGroupName, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume)
{
  mSoundSystem->SetChannelGroup3DCone(aGroupName, aOrientation, aInsideAngle, aOutsideAngle, aOutsideVolume);
}

/**
 * @brief Set 3D attenuation for channel group
 * @param aGroupName
 * @param aPoints
 */
void SoundManager::SetChannelGroup3DAttenuation(HashString const &aGroupName, std::vector<Vector3> const &aPoints)
{
  mSoundSystem->SetChannelGroup3DAttenuation(aGroupName, aPoints);
}

/**
 * @brief Set 3D min max distance for channel group
 * @param aGroupName
 * @param aMinDistance
 * @param aMaxDistance
 */
void SoundManager::SetChannelGroup3DMinMaxDistance(HashString const &aGroupName, float const aMinDistance, float const aMaxDistance)
{
  mSoundSystem->SetChannelGroup3DMinMaxDistance(aGroupName, aMinDistance, aMaxDistance);
}

/**
 * @brief Set occlusion for channel group
 * @param aGroupName
 * @param aDirectOcclusion
 * @param aReverbOcclusion
 */
void SoundManager::SetChannelGroup3DOcclusion(HashString const &aGroupName, float const aDirectOcclusion, float const aReverbOcclusion)
{
  mSoundSystem->SetChannelGroup3DOcclusion(aGroupName, aDirectOcclusion, aReverbOcclusion);
}

/**
 * @brief Set spread for channel group
 * @param aGroupName
 * @param aAngle
 */
void SoundManager::SetChannelGroup3DSpread(HashString const &aGroupName, float const aAngle)
{
  mSoundSystem->SetChannelGroup3DSpread(aGroupName, aAngle);
}

/**
 * @brief Return true if DSP exists
 * @param aName DSP name
 * @return True if exists
 */
bool SoundManager::HasDSP(HashString const &aName)
{
  return mSoundSystem->HasDSP(aName);
}

/**
 * @brief Create new DSP (Digital Signal Processor)
 * @param aName
 * @param aType
 * @return 
 */
DSP* SoundManager::CreateDSP(HashString const &aName, DSP_Type const &aType)
{
  DSP *dsp = mSoundSystem->CreateDSP(aName, aType);
  if(!dsp)
  {
    return nullptr;
  }
  
  mDSPContainer[dsp->GetName().ToHash()] = dsp;
  return dsp;
}

/**
 * @brief Get DSP from channel
 * @param aChannel
 * @param aIndex
 * @return 
 */
DSP* SoundManager::GetDSPFromChannel(int aChannel, int aIndex)
{
  HashString name = Common::IntToString(aChannel) + Common::IntToString(aIndex);
  if(mDSPContainer.find(name.ToHash()) != mDSPContainer.end())
    return mDSPContainer[name.ToHash()];
  
  DSP *dsp = mSoundSystem->GetDSPFromChannel(aChannel, aIndex);
  mDSPContainer[dsp->GetName().ToHash()] = dsp;
  return dsp;
}

/**
 * @brief Get DSP from channel group
 * @param aGroupName
 * @param aIndex
 * @return 
 */
DSP* SoundManager::GetDSPFromChannelGroup(HashString const &aGroupName, int aIndex)
{
  HashString name = aGroupName + Common::IntToString(aIndex);
  if(mDSPContainer.find(name.ToHash()) != mDSPContainer.end())
    return mDSPContainer[name.ToHash()];
    
  DSP *dsp = mSoundSystem->GetDSPFromChannelGroup(aGroupName, aIndex);
  mDSPContainer[dsp->GetName().ToHash()] = dsp;
  return dsp;
}

/**
 * @brief Delete DSP
 * @param aDSP
 */
void SoundManager::DeleteDSP(DSP *aDSP)
{
  if(mSoundSystem->DeleteDSP(aDSP))
  {
    // Delete all uses of dsp for individual sounds.
    for(SoundDSPContainerIt it = mSoundsToDSPs.begin(); it != mSoundsToDSPs.end(); ++it)
    {
      SoundDSPInfoVector dsps = it->second;
      for(SoundDSPInfoVectorIt it2 = dsps.begin(); it2 != dsps.end();)
      {
        SoundDSPInfo *info = *it2;
        if(info->mDSPName == aDSP->GetName())
        {
          delete info;
          it2 = dsps.erase(it2);
        }
        else
        {
          ++it2;
        }
      }
    }
    
    // Delete all uses of dsp for groups.
    for(SoundDSPContainerIt it = mGroupsToDSPs.begin(); it != mGroupsToDSPs.end(); ++it)
    {
      SoundDSPInfoVector dsps = it->second;
      for(SoundDSPInfoVectorIt it2 = dsps.begin(); it2 != dsps.end();)
      {
        SoundDSPInfo *info = *it2;
        if(info->mDSPName == aDSP->GetName())
        {
          delete info;
          it2 = dsps.erase(it2);
        }
        else
        {
          ++it2;
        }
      }
    }
    
    mDSPContainer.erase(aDSP->GetName().ToHash());
    delete aDSP;
  }
}

/**
 * @brief Add DSP to a channel
 * @param aDSP
 * @param aChannel
 * @param aIndex
 */
void SoundManager::AddDSPToChannel(DSP *aDSP, int const aChannel, int const aIndex)
{
  mSoundSystem->AddDSPToChannel(aDSP, aChannel, aIndex);
}

/**
 * @brief Add DSP to a channel group
 * @param aDSP
 * @param aGroupName
 * @param aIndex
 */
void SoundManager::AddDSPToChannelGroup(DSP *aDSP, HashString const& aGroupName, int const aIndex)
{
  mSoundSystem->AddDSPToChannelGroup(aDSP, aGroupName, aIndex);
}

/**
 * @brief Remove DSP from a channel
 * @param aDSP
 * @param aChannel
 */
void SoundManager::RemoveDSPFromChannel(DSP *aDSP, int const aChannel)
{
  mSoundSystem->RemoveDSPFromChannel(aDSP, aChannel);
}

/**
 * @brief Remove DSP from a channel group
 * @param aDSP
 * @param aGroupName
 */
void SoundManager::RemoveDSPFromChannelGroup(DSP *aDSP, HashString const& aGroupName)
{
  mSoundSystem->RemoveDSPFromChannelGroup(aDSP, aGroupName);
}

/**
 * @brief Set bus mute state.
 * @param aBusName Name of bus.
 * @param aMute Mute state.
 */
void SoundManager::SetBusMuteState(HashString const &aBusName, bool const aMute)
{
  mSoundSystem->SetBusMuteState(aBusName, aMute);
}

/**
 * @brief Set bus pause state.
 * @param aBusName Name of bus.
 * @param aPause Pause state.
 */
void SoundManager::SetBusPauseState(HashString const &aBusName, bool const aPause)
{
  mSoundSystem->SetBusPauseState(aBusName, aPause);
}

/**
 * @brief Set bus volume.
 * @param aBusName Name of bus.
 * @param aVolume Volume level.
 */
void SoundManager::SetBusVolume(HashString const &aBusName, float const aVolume)
{
  mSoundSystem->SetBusVolume(aBusName, aVolume);
}

/**
 * @brief Stop bus.
 * @param aBusName Name of bus.
 */
void SoundManager::StopBus(HashString const &aBusName)
{
  mSoundSystem->StopBus(aBusName);
}

/**
 * @brief Fade out bus.
 * @param aBusName Name of bus.
 */
void SoundManager::FadeBus(HashString const &aBusName)
{
  mSoundSystem->FadeBus(aBusName);
}

/**
 * @brief Lock bus.
 * @param aBusName Name of bus.
 */
void SoundManager::LockBus(HashString const &aBusName)
{
  mSoundSystem->LockBus(aBusName);
}

/**
 * @brief Unlock bus.
 * @param aBusName Name of bus.
 */
void SoundManager::UnlockBus(HashString const &aBusName)
{
  mSoundSystem->UnlockBus(aBusName);
}

/**
 * @brief Update loop.
 */
void SoundManager::Update()
{
  mSoundSystem->Update(GetOwningApp()->GetAppStep());
}

/**
 * @brief Does nothing for now.
 * @param aMessage
 */
void SoundManager::SendMessage(Message const &aMessage)
{
}

/**
 * @brief Does nothing for now.
 * @param aMessage
 */
void SoundManager::ProcessDelayedMessage(Message *aMessage)
{
}

/**
 * @brief Play sounds from LUA
 */
void SoundManager::SerializeLUA()
{
  SLB::Class<SoundManager>("SoundManager").inherits<Manager>()
      .set("PlaySound", &SoundManager::PlaySound)
      .set("StopChannel", &SoundManager::StopChannel)
      .set("SetSoundVolume", &SoundManager::SetChannelVolume)
      .set("ResumeChannel", &SoundManager::ResumeChannel)
      .set("PauseChannel", &SoundManager::PauseChannel)
      .set("PlayChannelGroup", &SoundManager::PlayChannelGroup);
}

/**
 * @brief Get alias if available for name.
 * @param aName
 * @return 
 */
HashString SoundManager::GetAliasIfAvailable(HashString const &aName)
{
  if(mAliases.find(aName.ToHash()) == mAliases.end())
  {
    return aName;
  }
  return mAliases[aName.ToHash()];
}

/**
 * @brief Load sounds from SoundAliases.txt file.
 */
void SoundManager::LoadSounds()
{
  int index = 0;
  HashString const bankStr = "Bank_";
  HashString const aliasStr = "Alias_";
  HashString const soundStr = "Sound_";
  HashString const dspStr = "DSP_";
  HashString const groupStr = "Group_";
  HashString curIndex = dspStr + Common::IntToString(index);
  HashString const fileName = "SoundAliases.txt";
  
  Parser *parser = ParserFactory::CreateInputParser("Sounds", fileName);
  ParserNode *head = parser->GetBaseRoot();
  while(head->Find(curIndex))
  {
    ParserNode *curDsp = head->Find(curIndex);
    HashString name = curDsp->Find("Name")->GetValue();
    HashString type = curDsp->Find("Type")->GetValue();
    
    DSP* dsp = nullptr;
    if(type == "MIXER")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_MIXER);
    }
    else if(type == "OSCILLATOR")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_OSCILLATOR);
    }
    else if(type == "LOWPASS")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_LOWPASS);
    }
    else if(type == "ITLOWPASS")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_ITLOWPASS);
    }
    else if(type == "HIGHPASS")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_HIGHPASS);
    }
    else if(type == "ECHO")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_ECHO);
    }
    else if(type == "FADER")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_FADER);
    }
    else if(type == "FLANGE")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_FLANGE);
    }
    else if(type == "DISTORTION")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_DISTORTION);
    }
    else if(type == "NORMALIZE")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_NORMALIZE);
    }
    else if(type == "LIMITER")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_LIMITER);
    }
    else if(type == "PARAMEQ")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_PARAMEQ);
    }
    else if(type == "PITCHSHIFT")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_PITCHSHIFT);
    }
    else if(type == "CHORUS")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_CHORUS);
    }
    else if(type == "VSTPLUGIN")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_VSTPLUGIN);
    }
    else if(type == "WINAMPPLUGIN")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_WINAMPPLUGIN);
    }
    else if(type == "ITECHO")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_ITECHO);
    }
    else if(type == "COMPRESSOR")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_COMPRESSOR);
    }
    else if(type == "SFXREVERB")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_SFXREVERB);
    }
    else if(type == "LOWPASS_SIMPLE")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_LOWPASS_SIMPLE);
    }
    else if(type == "DELAY")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_DELAY);
    }
    else if(type == "TREMOLO")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_TREMOLO);
    }
    else if(type == "LADSPAPLUGIN")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_LADSPAPLUGIN);
    }
    else if(type == "SEND")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_SEND);
    }
    else if(type == "RETURN")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_RETURN);
    }
    else if(type == "HIGHPASS_SIMPLE")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_HIGHPASS_SIMPLE);
    }
    else if(type == "PAN")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_PAN);
    }
    else if(type == "THREE_EQ")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_THREE_EQ);
    }
    else if(type == "FFT")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_FFT);
    }
    else if(type == "LOUDNESS_METER")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_LOUDNESS_METER);
    }
    else if(type == "ENVELOPEFOLLOWER")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_ENVELOPEFOLLOWER);
    }
    else if(type == "CONVOLUTIONREVERB")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_CONVOLUTIONREVERB);
    }
    else if(type == "CHANNELMIX")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_CHANNELMIX);
    }
    else if(type == "TRANSCEIVER")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_TRANSCEIVER);
    }
    else if(type == "OBJECTPAN")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_OBJECTPAN);
    }
    else if(type == "MULTIBAND_EQ")
    {
      dsp = CreateDSP(name, DSP_Type::DSP_TYPE_MULTIBAND_EQ);
    }
    else
    {
      DebugLogPrint("Invalid DSP type %s passed in.", type.ToCharArray());
      assert(!"Invalid DSP type passed in.");
    }
    
    if(dsp)
    {
      dsp->Deserialize(curDsp);
    }
    
    ++index;
    curIndex = dspStr + Common::IntToString(index);
  }
  
  // Banks
  index = 0;
  curIndex = bankStr + Common::IntToString(index);
  while(head->Find(curIndex))
  {
    ParserNode *curBank = head->Find(curIndex);
    SoundSystem::SoundSource source = SoundSystem::SoundSource::DEFAULT;
    
    HashString fileName = curBank->Find("File")->GetValue();
    if(curBank->Find("Source") && curBank->Find("Source")->GetValue() == "Nonblocking")
      source = SoundSystem::SoundSource::NONBLOCKING;
    else if(curBank->Find("Source") && curBank->Find("Source")->GetValue() == "Decompress")
      source = SoundSystem::SoundSource::DECOMPRESS;
    LoadSoundBank(fileName, source);
    ++index;
    curIndex = bankStr + Common::IntToString(index);
  }
  
  // Aliases
  index = 0;
  curIndex = aliasStr + Common::IntToString(index);
  while(head->Find(curIndex))
  {
    ParserNode *curAlias = head->Find(curIndex);
    HashString key = curAlias->Find("Alias")->GetValue();
    HashString value = curAlias->Find("Filename")->GetValue();
    mAliases[key.ToHash()] = value;
    ++index;
    curIndex = aliasStr + Common::IntToString(index);
  }
  
  // Sound aliasing
  index = 0;
  curIndex = soundStr + Common::IntToString(index);
  while(head->Find(curIndex))
  {
    ParserNode *curSound = head->Find(curIndex);
    SoundSystem::SoundSource source = SoundSystem::SoundSource::DEFAULT;
    float volume = 1.0f;
    
    HashString name = curSound->Find("Name")->GetValue();
    if(curSound->Find("Source") && curSound->Find("Source")->GetValue() == "Stream")
      source = SoundSystem::SoundSource::STREAM;
    else if(curSound->Find("Source") && curSound->Find("Source")->GetValue() == "Nonblocking")
      source = SoundSystem::SoundSource::NONBLOCKING;
    else if(curSound->Find("Source") && curSound->Find("Source")->GetValue() == "Decompress")
      source = SoundSystem::SoundSource::DECOMPRESS;
    if(curSound->Find("Volume"))
      volume = curSound->Find("Volume")->GetValue().ToFloat();
    CreateSound(curSound->Find("File")->GetValue(), name, volume, source);
    
    int dspIndex = 0;
    HashString curDsp = dspStr + Common::IntToString(dspIndex);
    while(curSound->Find(curDsp))
    {
      ParserNode *dspNode = curSound->Find(curDsp);
      SoundDSPInfo *info = new SoundDSPInfo(dspNode->Find("Index")->GetValue().ToInt(), dspNode->Find("Name")->GetValue());
      mSoundsToDSPs[name.ToHash()].push_back(info);
      ++dspIndex;
      curDsp = dspStr + Common::IntToString(dspIndex);
    }
    
    ++index;
    curIndex = soundStr + Common::IntToString(index);
  }
  
  // Grouping
  index = 0;
  curIndex = groupStr + Common::IntToString(index);
  while(head->Find(curIndex))
  {
    ParserNode *curGroup = head->Find(curIndex);
    HashString groupName = curGroup->Find("Name")->GetValue();
    CreateChannelGroup(groupName);
    
    // Add sounds
    int soundNameIndex = 0;
    HashString curSoundName = soundStr + Common::IntToString(soundNameIndex);
    while(curGroup->Find(curSoundName))
    {
      AddSoundNameToGroup(groupName, curGroup->Find(curSoundName)->GetValue());
      ++soundNameIndex;
      curSoundName = soundStr + Common::IntToString(soundNameIndex);
    }
    
    // Add DSPs
    int dspIndex = 0;
    HashString curDsp = dspStr + Common::IntToString(dspIndex);
    while(curGroup->Find(curDsp))
    {
      ParserNode *dspNode = curGroup->Find(curDsp);
      SoundDSPInfo *info = new SoundDSPInfo(dspNode->Find("Index")->GetValue().ToInt(), dspNode->Find("Name")->GetValue());
      mGroupsToDSPs[groupName.ToHash()].push_back(info);
      ++dspIndex;
      curDsp = dspStr + Common::IntToString(dspIndex);
    }
    
    ++index;
    curIndex = groupStr + Common::IntToString(index);
  }
}
