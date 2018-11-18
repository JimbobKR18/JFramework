#include "FMODStudioSoundSystem.h"

FMODStudioSoundSystem::FMODStudioSoundSystem() : mFMODStudioSystem(nullptr), mBanks(), mEventDescriptions(),
  mEventInstances(), mGroups(), mCurrentSound(0)
{
  FMOD_RESULT result = FMOD::Studio::System::create(&mFMODStudioSystem);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD Studio error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD Studio failed to initialize.");
  }
  
  result = mFMODStudioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD failed to initialize.");
  }
}

FMODStudioSoundSystem::~FMODStudioSoundSystem()
{
  mFMODStudioSystem->release();
}

/**
 * @brief Get FMOD DSP by name
 * @param aName
 * @return 
 */
FMOD::DSP* FMODStudioSoundSystem::GetDSP(HashString const &aName)
{
  return nullptr;
}

/**
 * @brief Update loop
 * @param aDT
 */
void FMODStudioSoundSystem::Update(float const aDT)
{
  mFMODStudioSystem->update();
  
  for(EventInstanceIt it = mEventInstances.begin(); it != mEventInstances.end();)
  {
    FMOD_STUDIO_PLAYBACK_STATE state;
    it->second->getPlaybackState(&state);
    if(state == FMOD_STUDIO_PLAYBACK_STOPPED)
    {
      RemoveChannelFromGroups(it->first);
      it = mEventInstances.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

/**
 * @brief Load sound bank
 * @param aFilename
 * @param aSource
 */
void FMODStudioSoundSystem::LoadSoundBank(HashString const& aFilename, SoundSource const& aSource)
{
  HashString fileName = Common::RelativePath("Sounds", aFilename);
  FMOD::Studio::Bank *bank = nullptr;
  FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMOD_STUDIO_LOAD_BANK_NORMAL;
  switch(aSource)
  {
    case DEFAULT:
      break;
    case NONBLOCKING:
      flags = FMOD_STUDIO_LOAD_BANK_NONBLOCKING;
      break;
    case DECOMPRESS:
      flags = FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES;
      break;
    default:
      assert(!"Invalid bank loading state passed into FMODStudioSoundSystem::CreateSound");
      break;
  }
  
  FMOD_RESULT result = mFMODStudioSystem->loadBankFile(fileName.ToCharArray(), flags, &bank);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD failed to create sound.");
  }
  
  mBanks[aFilename.ToHash()] = bank;
}

/**
 * @brief Unload a sound bank
 * @param aFilename
 */
void FMODStudioSoundSystem::UnloadSoundBank(HashString const& aFilename)
{
  if(mBanks.find(aFilename.ToHash()) == mBanks.end())
  {
    assert(!"Sound bank not found.");
  }
  
  mBanks[aFilename.ToHash()]->unload();
}

/**
 * @brief Create sound
 * @param aFilename
 * @param aAlias
 * @param aDefaultVolume
 * @param aSource
 */
void FMODStudioSoundSystem::CreateSound(HashString const& aFilename, float const &aDefaultVolume, SoundSource const& aSource)
{
  int key = aFilename.ToHash();
  if(mEventDescriptions.find(key) == mEventDescriptions.end())
  {
    FMOD::Studio::EventDescription* description = nullptr;
    mFMODStudioSystem->getEvent(aFilename.ToCharArray(), &description);
    mEventDescriptions[key] = description;
  }
}

/**
 * @brief Delete sound
 * @param aName
 */
void FMODStudioSoundSystem::DeleteSound(HashString const& aName)
{
  int key = aName.ToHash();
  
  if(mBanks.find(key) == mBanks.end())
  {
    assert(!"Sound bank not found!");
  }
  
  mBanks[key]->unload();
  mBanks.erase(key);
}

/**
 * @brief Play sound
 * @param aName
 * @param aNumLoops
 * @return 
 */
int FMODStudioSoundSystem::PlaySound(HashString const& aName, int const aNumLoops)
{
  int key = aName.ToHash();
  if(mEventDescriptions.find(key) == mEventDescriptions.end())
  {
    FMOD::Studio::EventDescription* description = nullptr;
    mFMODStudioSystem->getEvent(aName.ToCharArray(), &description);
    mEventDescriptions[key] = description;
  }
  
  int index = mCurrentSound++;
  FMOD::Studio::EventInstance* instance = nullptr;
  mEventDescriptions[key]->createInstance(&instance);
  mEventInstances[index] = instance;
  
  if(aNumLoops == 0)
  {
    instance->release();
  }
  
  return index;
}

/**
 * @brief Resume channel
 * @param aChannel
 */
void FMODStudioSoundSystem::ResumeChannel(int const aChannel)
{
  if(mEventInstances.find(aChannel) == mEventInstances.end())
  {
    DebugLogPrint("Sound event instance %d not found.", aChannel);
    return;
  }
  
  mEventInstances[aChannel]->setPaused(false);
}

/**
 * @brief Pause channel
 * @param aChannel
 */
void FMODStudioSoundSystem::PauseChannel(int const aChannel)
{
  if(mEventInstances.find(aChannel) == mEventInstances.end())
  {
    DebugLogPrint("Sound event instance %d not found.", aChannel);
    return;
  }
  
  mEventInstances[aChannel]->setPaused(true);
}

/**
 * @brief Set master volume
 * @param aVolume
 */
void FMODStudioSoundSystem::SetMasterVolume(float const aVolume)
{
  for(EventInstanceIt it = mEventInstances.begin(); it != mEventInstances.end();)
  {
    // TODO maintain volume balance
    it->second->setVolume(aVolume);
  }
}

/**
 * @brief Stop all sounds
 */
void FMODStudioSoundSystem::StopAllChannels()
{
  for(EventInstanceIt it = mEventInstances.begin(); it != mEventInstances.end(); ++it)
  {
    it->second->stop(FMOD_STUDIO_STOP_IMMEDIATE);
  }
  mEventInstances.clear();
}

/**
 * @brief Set channel volume
 * @param aChannel
 * @param aVolume
 */
void FMODStudioSoundSystem::SetChannelVolume(int const aChannel, float const aVolume)
{
  if(mEventInstances.find(aChannel) == mEventInstances.end())
  {
    DebugLogPrint("Sound event instance %d not found.", aChannel);
    return;
  }
  
  mEventInstances[aChannel]->setVolume(aVolume);
}

/**
 * @brief Stop channel
 * @param aChannel
 */
void FMODStudioSoundSystem::StopChannel(int const aChannel)
{
  if(mEventInstances.find(aChannel) == mEventInstances.end())
  {
    DebugLogPrint("Sound event instance %d not found.", aChannel);
    return;
  }
  
  mEventInstances[aChannel]->stop(FMOD_STUDIO_STOP_IMMEDIATE);
}

/**
 * @brief Fade channel from start to end.
 * @param aChannel Channel to fade.
 * @param aTime Time in samples.
 * @param aStart Starting volume.
 * @param aEnd Ending volume.
 */
void FMODStudioSoundSystem::FadeChannel(int const aChannel, int const aTime, float const aStart, float const aEnd)
{
  if(mEventInstances.find(aChannel) == mEventInstances.end())
  {
    DebugLogPrint("Sound event instance %d not found.", aChannel);
    return;
  }
  
  mEventInstances[aChannel]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

/**
 * @brief Delay channel start and end. TODO broken.
 * @param aChannel Channel to delay.
 * @param aStartDelay Time in samples.
 * @param aEndDelay Time in samples.
 * @param aStopChannels True if channel should stop when sound data ends.
 */
void FMODStudioSoundSystem::DelayChannel(int const aChannel, int const aStartDelay, int const aEndDelay, bool const aStopChannels)
{
  // Does nothing
}

/**
 * @brief Get channel property value.
 * @param aChannel Channel id.
 * @param aName Name of property.
 * @return Property value.
 */
float FMODStudioSoundSystem::GetChannelProperty(int const aChannel, HashString const &aName)
{
  if(mEventInstances.find(aChannel) == mEventInstances.end())
  {
    DebugLogPrint("Sound event instance %d not found.", aChannel);
    return 0;
  }
  
  float value = 0;
  mEventInstances[aChannel]->getParameterValue(aName.ToCharArray(), &value, nullptr);
  return value;
}

/**
 * @brief Set channel property value.
 * @param aChannel Channel id.
 * @param aName Name of property.
 * @param aValue Value of property.
 */
void FMODStudioSoundSystem::SetChannelProperty(int const aChannel, HashString const &aName, float const aValue)
{
  if(mEventInstances.find(aChannel) == mEventInstances.end())
  {
    DebugLogPrint("Sound event instance %d not found.", aChannel);
    return;
  }
  
  mEventInstances[aChannel]->setParameterValue(aName.ToCharArray(), aValue);
}

/**
 * @brief Set channel frequency
 * @param aChannel
 * @param aFrequency
 */
void FMODStudioSoundSystem::SetChannelFrequency(int const aChannel, float const aFrequency)
{
}

/**
 * @brief Set 3D attributes for channel
 * @param aGroupName
 * @param aPos
 * @param aVel
 * @param aAltPanPos
 */
void FMODStudioSoundSystem::SetChannel3DAttributes(int const aChannel, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos)
{
}

/**
 * @brief Set cone for channel
 * @param aGroupName
 * @param aOrientation
 * @param aInsideAngle
 * @param aOutsideAngle
 * @param aOutsideVolume
 */
void FMODStudioSoundSystem::SetChannel3DCone(int const aChannel, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume)
{
}

/**
 * @brief Set 3D attenuation for channel
 * @param aGroupName
 * @param aPoints
 */
void FMODStudioSoundSystem::SetChannel3DAttenuation(int const aChannel, std::vector<Vector3> const &aPoints)
{
}

/**
 * @brief Set 3D min max distance for channel
 * @param aGroupName
 * @param aMinDistance
 * @param aMaxDistance
 */
void FMODStudioSoundSystem::SetChannel3DMinMaxDistance(int const aChannel, float const aMinDistance, float const aMaxDistance)
{
}

/**
 * @brief Set occlusion for channel
 * @param aGroupName
 * @param aDirectOcclusion
 * @param aReverbOcclusion
 */
void FMODStudioSoundSystem::SetChannel3DOcclusion(int const aChannel, float const aDirectOcclusion, float const aReverbOcclusion)
{
}

/**
 * @brief Set spread for channel
 * @param aGroupName
 * @param aAngle
 */
void FMODStudioSoundSystem::SetChannel3DSpread(int const aChannel, float const aAngle)
{
}

/**
 * @brief Create channel group
 * @param aGroupName
 */
void FMODStudioSoundSystem::CreateChannelGroup(HashString const &aGroupName)
{
  if(mGroups.find(aGroupName.ToHash()) != mGroups.end())
  {
    DebugLogPrint("Channel Group %s already exists.", aGroupName.ToCharArray());
    return;
  }
  
  mGroups[aGroupName.ToHash()] = InstanceContainer();
}

/**
 * @brief Add channel to channel group
 * @param aGroupName
 * @param aChannel
 */
void FMODStudioSoundSystem::AddChannelToGroup(HashString const &aGroupName, int const aChannel)
{
  if(mGroups.find(aGroupName.ToHash()) == mGroups.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  mGroups[aGroupName.ToHash()].insert(aChannel);
}

/**
 * @brief Remove channel from groupings.
 * @param aChannel Channel to remove.
 */
void FMODStudioSoundSystem::RemoveChannelFromGroups(int const aChannel)
{
  for(InstanceGroupIt it = mGroups.begin(); it != mGroups.end(); ++it)
  {
    it->second.erase(aChannel);
  }
}

/**
 * @brief Set channel group volume
 * @param aGroupName
 * @param aVolume
 */
void FMODStudioSoundSystem::SetChannelGroupVolume(HashString const &aGroupName, float const aVolume)
{
  if(mGroups.find(aGroupName.ToHash()) == mGroups.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  for(InstanceIt it = mGroups[aGroupName.ToHash()].begin(); it != mGroups[aGroupName.ToHash()].end(); ++it)
  {
    if(mEventInstances.find(*it) == mEventInstances.end())
    {
      continue;
    }
    
    mEventInstances[*it]->setVolume(aVolume);
  }
}

/**
 * @brief Stop channel group
 * @param aGroupName
 */
void FMODStudioSoundSystem::StopChannelGroup(HashString const &aGroupName)
{
  if(mGroups.find(aGroupName.ToHash()) == mGroups.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  for(InstanceIt it = mGroups[aGroupName.ToHash()].begin(); it != mGroups[aGroupName.ToHash()].end(); ++it)
  {
    if(mEventInstances.find(*it) == mEventInstances.end())
    {
      continue;
    }
    
    mEventInstances[*it]->stop(FMOD_STUDIO_STOP_IMMEDIATE);
  }
}

/**
 * @brief Fade channel group from start to end.
 * @param aGroupName Channel group to fade.
 * @param aTime Time in samples.
 * @param aStart Starting volume.
 * @param aEnd Ending volume.
 */
void FMODStudioSoundSystem::FadeChannelGroup(HashString const &aGroupName, int const aTime, float const aStart, float const aEnd)
{
  if(mGroups.find(aGroupName.ToHash()) == mGroups.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  for(InstanceIt it = mGroups[aGroupName.ToHash()].begin(); it != mGroups[aGroupName.ToHash()].end(); ++it)
  {
    if(mEventInstances.find(*it) == mEventInstances.end())
    {
      continue;
    }
    
    mEventInstances[*it]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
  }
}

/**
 * @brief Set 3D attributes for channel group
 * @param aGroupName
 * @param aPos
 * @param aVel
 * @param aAltPanPos
 */
void FMODStudioSoundSystem::SetChannelGroup3DAttributes(HashString const &aGroupName, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos)
{
}

/**
 * @brief Set cone for channel group
 * @param aGroupName
 * @param aOrientation
 * @param aInsideAngle
 * @param aOutsideAngle
 * @param aOutsideVolume
 */
void FMODStudioSoundSystem::SetChannelGroup3DCone(HashString const &aGroupName, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume)
{
}

/**
 * @brief Set 3D attenuation for channel group
 * @param aGroupName
 * @param aPoints
 */
void FMODStudioSoundSystem::SetChannelGroup3DAttenuation(HashString const &aGroupName, std::vector<Vector3> const &aPoints)
{
}

/**
 * @brief Set 3D min max distance for channel group
 * @param aGroupName
 * @param aMinDistance
 * @param aMaxDistance
 */
void FMODStudioSoundSystem::SetChannelGroup3DMinMaxDistance(HashString const &aGroupName, float const aMinDistance, float const aMaxDistance)
{
}

/**
 * @brief Set occlusion for channel group
 * @param aGroupName
 * @param aDirectOcclusion
 * @param aReverbOcclusion
 */
void FMODStudioSoundSystem::SetChannelGroup3DOcclusion(HashString const &aGroupName, float const aDirectOcclusion, float const aReverbOcclusion)
{
}

/**
 * @brief Set spread for channel group
 * @param aGroupName
 * @param aAngle
 */
void FMODStudioSoundSystem::SetChannelGroup3DSpread(HashString const &aGroupName, float const aAngle)
{
}

/**
 * @brief Create DSP (Digital Signal Processor)
 * @param aName
 * @param aType
 * @return 
 */
DSP* FMODStudioSoundSystem::CreateDSP(HashString const &aName, DSP_Type const &aType)
{
  return nullptr;
}

/**
 * @brief Get DSP from channel
 * @param aChannel
 * @param aIndex
 * @return 
 */
DSP* FMODStudioSoundSystem::GetDSPFromChannel(int aChannel, int aIndex)
{
  return nullptr;
}

/**
 * @brief Get DSP from channel group
 * @param aGroupName
 * @param aIndex
 * @return 
 */
DSP* FMODStudioSoundSystem::GetDSPFromChannelGroup(HashString const &aGroupName, int aIndex)
{
  return nullptr;
}

/**
 * @brief Delete DSP
 * @param aDSP
 * @return True if DSP was successfully deleted
 */
bool FMODStudioSoundSystem::DeleteDSP(DSP *aDSP)
{
  return false;
}

/**
 * @brief Add DSP to channel
 * @param aDSP
 * @param aChannel
 * @param aIndex
 */
void FMODStudioSoundSystem::AddDSPToChannel(DSP *aDSP, int const aChannel, int const aIndex)
{
}

/**
 * @brief Add DSP to channel group
 * @param aDSP
 * @param aGroupName
 * @param aIndex
 */
void FMODStudioSoundSystem::AddDSPToChannelGroup(DSP *aDSP, HashString const& aGroupName, int const aIndex)
{
}

/**
 * @brief Remove DSP from channel
 * @param aDSP
 * @param aChannel
 */
void FMODStudioSoundSystem::RemoveDSPFromChannel(DSP *aDSP, int const aChannel)
{
}

/**
 * @brief Remove DSP from channel group
 * @param aDSP
 * @param aGroupName
 */
void FMODStudioSoundSystem::RemoveDSPFromChannelGroup(DSP *aDSP, HashString const& aGroupName)
{
}

/**
 * @brief Set bus mute state.
 * @param aBusName Name of bus.
 * @param aMute Mute state.
 */
void FMODStudioSoundSystem::SetBusMuteState(HashString const &aBusName, bool const aMute)
{
  FMOD::Studio::Bus *bus = nullptr;
  mFMODStudioSystem->getBus(aBusName.ToCharArray(), &bus);
  if(bus == nullptr)
  {
    DebugLogPrint("Bus %s not found.", aBusName.ToCharArray());
    return;
  }
  
  bus->setMute(aMute);
}

/**
 * @brief Set bus pause state.
 * @param aBusName Name of bus.
 * @param aPause Pause state.
 */
void FMODStudioSoundSystem::SetBusPauseState(HashString const &aBusName, bool const aPause)
{
  FMOD::Studio::Bus *bus = nullptr;
  mFMODStudioSystem->getBus(aBusName.ToCharArray(), &bus);
  if(bus == nullptr)
  {
    DebugLogPrint("Bus %s not found.", aBusName.ToCharArray());
    return;
  }
  
  bus->setPaused(aPause);
}

/**
 * @brief Set bus volume.
 * @param aBusName Name of bus.
 * @param aVolume Volume level.
 */
void FMODStudioSoundSystem::SetBusVolume(HashString const &aBusName, float const aVolume)
{
  FMOD::Studio::Bus *bus = nullptr;
  mFMODStudioSystem->getBus(aBusName.ToCharArray(), &bus);
  if(bus == nullptr)
  {
    DebugLogPrint("Bus %s not found.", aBusName.ToCharArray());
    return;
  }
  
  bus->setVolume(aVolume);
}

/**
 * @brief Stop bus.
 * @param aBusName Name of bus.
 */
void FMODStudioSoundSystem::StopBus(HashString const &aBusName)
{
  FMOD::Studio::Bus *bus = nullptr;
  mFMODStudioSystem->getBus(aBusName.ToCharArray(), &bus);
  if(bus == nullptr)
  {
    DebugLogPrint("Bus %s not found.", aBusName.ToCharArray());
    return;
  }
  
  bus->stopAllEvents(FMOD_STUDIO_STOP_IMMEDIATE);
}

/**
 * @brief Fade out bus.
 * @param aBusName Name of bus.
 */
void FMODStudioSoundSystem::FadeBus(HashString const &aBusName)
{
  FMOD::Studio::Bus *bus = nullptr;
  mFMODStudioSystem->getBus(aBusName.ToCharArray(), &bus);
  if(bus == nullptr)
  {
    DebugLogPrint("Bus %s not found.", aBusName.ToCharArray());
    return;
  }
  
  bus->stopAllEvents(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

/**
 * @brief Lock bus.
 * @param aBusName Name of bus.
 */
void FMODStudioSoundSystem::LockBus(HashString const &aBusName)
{
  FMOD::Studio::Bus *bus = nullptr;
  mFMODStudioSystem->getBus(aBusName.ToCharArray(), &bus);
  if(bus == nullptr)
  {
    DebugLogPrint("Bus %s not found.", aBusName.ToCharArray());
    return;
  }
  
  bus->lockChannelGroup();
}

/**
 * @brief Unlock bus.
 * @param aBusName Name of bus.
 */
void FMODStudioSoundSystem::UnlockBus(HashString const &aBusName)
{
  FMOD::Studio::Bus *bus = nullptr;
  mFMODStudioSystem->getBus(aBusName.ToCharArray(), &bus);
  if(bus == nullptr)
  {
    DebugLogPrint("Bus %s not found.", aBusName.ToCharArray());
    return;
  }
  
  bus->unlockChannelGroup();
}
