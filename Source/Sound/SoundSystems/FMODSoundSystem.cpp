#include "FMODSoundSystem.h"
#include "FMOD_DSP.h"

FMODSound::FMODSound(FMOD::Sound* aSound, float const &aVolume) : mSound(aSound), mVolume(aVolume)
{
}

FMODSound::~FMODSound()
{
  mSound->release();
}

FMODSoundSystem::FMODSoundSystem() : mFMODStudioSystem(nullptr), mFMODSystem(nullptr), 
                                     mMasterChannelGroup(nullptr), mSoundContainer(), 
                                     mChannelGroupContainer(), mDSPContainer()
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
  
  mFMODStudioSystem->getLowLevelSystem(&mFMODSystem);
  mFMODSystem->getMasterChannelGroup(&mMasterChannelGroup);
  
  mChannelGroupContainer[Common::StringHashFunction("Master")] = mMasterChannelGroup;
}

FMODSoundSystem::~FMODSoundSystem()
{
  for(FMODSoundIt it = mSoundContainer.begin(); it != mSoundContainer.end(); ++it)
  {
    delete it->second;
  }
  for(FMODChannelGroupIt it = mChannelGroupContainer.begin(); it != mChannelGroupContainer.end(); ++it)
  {
    it->second->release();
  }
  for(FMODDSPIt it = mDSPContainer.begin(); it != mDSPContainer.end(); ++it)
  {
    it->second->release();
  }
  mSoundContainer.clear();
  mChannelGroupContainer.clear();
  mDSPContainer.clear();
  mFMODStudioSystem->release();
}

/**
 * @brief Get FMOD DSP by name
 * @param aName
 * @return 
 */
FMOD::DSP* FMODSoundSystem::GetDSP(HashString const &aName)
{
  return mDSPContainer[aName.ToHash()];
}

/**
 * @brief Update loop
 * @param aDT
 */
void FMODSoundSystem::Update(float const aDT)
{
  mFMODStudioSystem->update();
}

/**
 * @brief Load sound bank
 * @param aFilename
 * @param aSource
 */
void FMODSoundSystem::LoadSoundBank(HashString const& aFilename, SoundSource const& aSource)
{
  // Does nothing
}

/**
 * @brief Unload a sound bank
 * @param aFilename
 */
void FMODSoundSystem::UnloadSoundBank(HashString const& aFilename)
{
  // Does nothing
}

/**
 * @brief Create sound
 * @param aFilename
 * @param aAlias
 * @param aDefaultVolume
 * @param aSource
 */
void FMODSoundSystem::CreateSound(HashString const& aFilename, float const &aDefaultVolume, SoundSource const& aSource)
{
  HashString fileName = Common::RelativePath("Sounds", aFilename);
  FMOD::Sound *sound;
  
  FMOD_MODE mode = FMOD_DEFAULT;
  
  if(aSource == SoundSource::STREAM)
    mode = FMOD_CREATESTREAM;
  
  mode |= FMOD_LOOP_NORMAL;
  FMOD_RESULT result = mFMODSystem->createSound(fileName.ToCharArray(), mode, 0, &sound);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD failed to create sound.");
  }
  
  FMODSound *newSound = new FMODSound(sound, aDefaultVolume);
  mSoundContainer[aFilename.ToHash()] = newSound;
}

/**
 * @brief Delete sound
 * @param aName
 */
void FMODSoundSystem::DeleteSound(HashString const& aName)
{
  if(mSoundContainer.find(aName.ToHash()) == mSoundContainer.end())
    return;
  
  delete mSoundContainer[aName.ToHash()];
  mSoundContainer.erase(aName.ToHash());
}

/**
 * @brief Play sound
 * @param aName
 * @param aNumLoops
 * @return 
 */
int FMODSoundSystem::PlaySound(HashString const& aName, int const aNumLoops)
{
  FMOD::Channel *channel;
  int index = 0;
  FMOD_RESULT result = mFMODSystem->playSound(mSoundContainer[aName.ToHash()]->mSound, 0, false, &channel);
  channel->setLoopCount(aNumLoops);
  channel->setVolume(mSoundContainer[aName.ToHash()]->mVolume);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD error: (%s) (%d) %s\n", aName.ToCharArray(), result, FMOD_ErrorString(result));
    assert(!"FMOD failed to play sound.");
  }
  channel->getIndex(&index);
  return index;
}

/**
 * @brief Resume channel
 * @param aChannel
 */
void FMODSoundSystem::ResumeChannel(int const aChannel)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->setPaused(false);
}

/**
 * @brief Pause channel
 * @param aChannel
 */
void FMODSoundSystem::PauseChannel(int const aChannel)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->setPaused(true);
}

/**
 * @brief Set master volume
 * @param aVolume
 */
void FMODSoundSystem::SetMasterVolume(float const aVolume)
{
  mMasterChannelGroup->setVolume(aVolume);
}

/**
 * @brief Stop all sounds
 */
void FMODSoundSystem::StopAllChannels()
{
  mMasterChannelGroup->stop();
}

/**
 * @brief Set channel volume
 * @param aChannel
 * @param aVolume
 */
void FMODSoundSystem::SetChannelVolume(int const aChannel, float const aVolume)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->setVolume(aVolume);
}

/**
 * @brief Stop channel
 * @param aChannel
 */
void FMODSoundSystem::StopChannel(int const aChannel)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->stop();
}

/**
 * @brief Fade channel from start to end.
 * @param aChannel Channel to fade.
 * @param aTime Time in samples.
 * @param aStart Starting volume.
 * @param aEnd Ending volume.
 */
void FMODSoundSystem::FadeChannel(int const aChannel, int const aTime, float const aStart, float const aEnd)
{
  unsigned long long clock;
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  FMOD_RESULT result = channel->getDSPClock(nullptr, &clock);
  result = channel->addFadePoint(clock, aStart);
  result = channel->addFadePoint(clock + aTime, aEnd);
}

/**
 * @brief Delay channel start and end. TODO broken.
 * @param aChannel Channel to delay.
 * @param aStartDelay Time in samples.
 * @param aEndDelay Time in samples.
 * @param aStopChannels True if channel should stop when sound data ends.
 */
void FMODSoundSystem::DelayChannel(int const aChannel, int const aStartDelay, int const aEndDelay, bool const aStopChannels)
{
  FMOD::Channel *channel;
  unsigned long long start, end;
  bool stopChannels;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->getDelay(&start, &end, &stopChannels);
  start += aStartDelay;
  end += aEndDelay;
  channel->setDelay(aStartDelay, aEndDelay, aStopChannels);
}

/**
 * @brief Get channel property value.
 * @param aChannel Channel id.
 * @param aName Name of property.
 * @return Property value.
 */
float FMODSoundSystem::GetChannelProperty(int const aChannel, HashString const &aName)
{
  return 0;
}

/**
 * @brief Set channel property value.
 * @param aChannel Channel id.
 * @param aName Name of property.
 * @param aValue Value of property.
 */
void FMODSoundSystem::SetChannelProperty(int const aChannel, HashString const &aName, float const aValue)
{
  // Does nothing
}

/**
 * @brief Set channel frequency
 * @param aChannel
 * @param aFrequency
 */
void FMODSoundSystem::SetChannelFrequency(int const aChannel, float const aFrequency)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->setFrequency(aFrequency);
}

/**
 * @brief Set 3D attributes for channel
 * @param aGroupName
 * @param aPos
 * @param aVel
 * @param aAltPanPos
 */
void FMODSoundSystem::SetChannel3DAttributes(int const aChannel, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos)
{
  FMOD_VECTOR pos = {aPos.x, aPos.y, aPos.z};
  FMOD_VECTOR vel = {aVel.x, aVel.y, aVel.z};
  FMOD_VECTOR altPanPos = {aAltPanPos.x, aAltPanPos.y, aAltPanPos.z};
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->set3DAttributes(&pos, &vel, &altPanPos);
}

/**
 * @brief Set cone for channel
 * @param aGroupName
 * @param aOrientation
 * @param aInsideAngle
 * @param aOutsideAngle
 * @param aOutsideVolume
 */
void FMODSoundSystem::SetChannel3DCone(int const aChannel, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume)
{
  FMOD_VECTOR orientation = {aOrientation.x, aOrientation.y, aOrientation.z};
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->set3DConeOrientation(&orientation);
  channel->set3DConeSettings(aInsideAngle, aOutsideAngle, aOutsideVolume);
}

/**
 * @brief Set 3D attenuation for channel
 * @param aGroupName
 * @param aPoints
 */
void FMODSoundSystem::SetChannel3DAttenuation(int const aChannel, std::vector<Vector3> const &aPoints)
{
  std::vector<FMOD_VECTOR> points;
  for(std::vector<Vector3>::const_iterator it = aPoints.begin(); it != aPoints.end(); ++it)
  {
    points.push_back({it->x, it->y, it->z});
  }
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->set3DCustomRolloff(&points[0], aPoints.size());
}

/**
 * @brief Set 3D min max distance for channel
 * @param aGroupName
 * @param aMinDistance
 * @param aMaxDistance
 */
void FMODSoundSystem::SetChannel3DMinMaxDistance(int const aChannel, float const aMinDistance, float const aMaxDistance)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->set3DMinMaxDistance(aMinDistance, aMaxDistance);
}

/**
 * @brief Set occlusion for channel
 * @param aGroupName
 * @param aDirectOcclusion
 * @param aReverbOcclusion
 */
void FMODSoundSystem::SetChannel3DOcclusion(int const aChannel, float const aDirectOcclusion, float const aReverbOcclusion)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->set3DOcclusion(aDirectOcclusion, aReverbOcclusion);
}

/**
 * @brief Set spread for channel
 * @param aGroupName
 * @param aAngle
 */
void FMODSoundSystem::SetChannel3DSpread(int const aChannel, float const aAngle)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->set3DSpread(aAngle);
}

/**
 * @brief Return true if channel group exists
 * @param aGroupName Name of group
 * @return True if group exists
 */
bool FMODSoundSystem::HasChannelGroup(HashString const &aGroupName)
{
  return mChannelGroupContainer.find(aGroupName.ToHash()) != mChannelGroupContainer.end();
}

/**
 * @brief Create channel group
 * @param aGroupName
 */
void FMODSoundSystem::CreateChannelGroup(HashString const &aGroupName)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) != mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s already exists.", aGroupName.ToCharArray());
    return;
  }
  
  FMOD::ChannelGroup *channelGroup;
  mFMODSystem->createChannelGroup(aGroupName.ToCharArray(), &channelGroup);
  mChannelGroupContainer[aGroupName.ToHash()] = channelGroup;
}

/**
 * @brief Add channel to channel group
 * @param aGroupName
 * @param aChannel
 */
void FMODSoundSystem::AddChannelToGroup(HashString const &aGroupName, int const aChannel)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  
  channel->setChannelGroup(group);
}

/**
 * @brief Remove channel from groupings.
 * @param aChannel Channel to remove.
 */
void FMODSoundSystem::RemoveChannelFromGroups(int const aChannel)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  
  channel->setChannelGroup(nullptr);
}

/**
 * @brief Set channel group volume
 * @param aGroupName
 * @param aVolume
 */
void FMODSoundSystem::SetChannelGroupVolume(HashString const &aGroupName, float const aVolume)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  group->setVolume(aVolume);
}

/**
 * @brief Stop channel group
 * @param aGroupName
 */
void FMODSoundSystem::StopChannelGroup(HashString const &aGroupName)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  group->stop();
}

/**
 * @brief Fade channel group from start to end.
 * @param aGroupName Channel group to fade.
 * @param aTime Time in samples.
 * @param aStart Starting volume.
 * @param aEnd Ending volume.
 */
void FMODSoundSystem::FadeChannelGroup(HashString const &aGroupName, int const aTime, float const aStart, float const aEnd)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  unsigned long long clock;
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  FMOD_RESULT result = group->getDSPClock(nullptr, &clock);
  result = group->addFadePoint(clock, aStart);
  result = group->addFadePoint(clock + aTime, aEnd);
}

/**
 * @brief Set 3D attributes for channel group
 * @param aGroupName
 * @param aPos
 * @param aVel
 * @param aAltPanPos
 */
void FMODSoundSystem::SetChannelGroup3DAttributes(HashString const &aGroupName, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  FMOD_VECTOR pos = {aPos.x, aPos.y, aPos.z};
  FMOD_VECTOR vel = {aVel.x, aVel.y, aVel.z};
  FMOD_VECTOR altPanPos = {aAltPanPos.x, aAltPanPos.y, aAltPanPos.z};
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  group->set3DAttributes(&pos, &vel, &altPanPos);
}

/**
 * @brief Set cone for channel group
 * @param aGroupName
 * @param aOrientation
 * @param aInsideAngle
 * @param aOutsideAngle
 * @param aOutsideVolume
 */
void FMODSoundSystem::SetChannelGroup3DCone(HashString const &aGroupName, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  FMOD_VECTOR orientation = {aOrientation.x, aOrientation.y, aOrientation.z};
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  group->set3DConeOrientation(&orientation);
  group->set3DConeSettings(aInsideAngle, aOutsideAngle, aOutsideVolume);
}

/**
 * @brief Set 3D attenuation for channel group
 * @param aGroupName
 * @param aPoints
 */
void FMODSoundSystem::SetChannelGroup3DAttenuation(HashString const &aGroupName, std::vector<Vector3> const &aPoints)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  std::vector<FMOD_VECTOR> points;
  for(std::vector<Vector3>::const_iterator it = aPoints.begin(); it != aPoints.end(); ++it)
  {
    points.push_back({it->x, it->y, it->z});
  }
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  group->set3DCustomRolloff(&points[0], aPoints.size());
}

/**
 * @brief Set 3D min max distance for channel group
 * @param aGroupName
 * @param aMinDistance
 * @param aMaxDistance
 */
void FMODSoundSystem::SetChannelGroup3DMinMaxDistance(HashString const &aGroupName, float const aMinDistance, float const aMaxDistance)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  group->set3DMinMaxDistance(aMinDistance, aMaxDistance);
}

/**
 * @brief Set occlusion for channel group
 * @param aGroupName
 * @param aDirectOcclusion
 * @param aReverbOcclusion
 */
void FMODSoundSystem::SetChannelGroup3DOcclusion(HashString const &aGroupName, float const aDirectOcclusion, float const aReverbOcclusion)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  group->set3DOcclusion(aDirectOcclusion, aReverbOcclusion);
}

/**
 * @brief Set spread for channel group
 * @param aGroupName
 * @param aAngle
 */
void FMODSoundSystem::SetChannelGroup3DSpread(HashString const &aGroupName, float const aAngle)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  group->set3DSpread(aAngle);
}

/**
 * @brief Return true if DSP exists
 * @param aName DSP name
 * @return True if exists
 */
bool FMODSoundSystem::HasDSP(HashString const &aName)
{
  return mDSPContainer.find(aName.ToHash()) != mDSPContainer.end();
}

/**
 * @brief Create DSP (Digital Signal Processor)
 * @param aName
 * @param aType
 * @return 
 */
DSP* FMODSoundSystem::CreateDSP(HashString const &aName, DSP_Type const &aType)
{
  if(aType >= DSP_TYPE_MAX)
  {
    DebugLogPrint("DSP type %d exceeds FMOD max.", aType);
    assert(!"DSP type exceeds FMOD max.");
  }
  
  if(aType == DSP_Type::DSP_TYPE_UNKNOWN || aType == DSP_Type::DSP_TYPE_MAX)
  {
    assert(!"Invalid DSP Type passed into CreateDSP");
  }
  
  DSP *dsp = new FMOD_DSP(mFMODSystem, this, aName, aType);
  mDSPContainer[aName.ToHash()] = dynamic_cast<FMOD_DSP*>(dsp)->GetFMODDSP();

  return dsp;
}

/**
 * @brief Get DSP from channel
 * @param aChannel
 * @param aIndex
 * @return 
 */
DSP* FMODSoundSystem::GetDSPFromChannel(int aChannel, int aIndex)
{
  FMOD::DSP *dsp;
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->getDSP(aIndex, &dsp);
  
  DSP* fmodDSP = new FMOD_DSP(dsp, this, Common::IntToString(aChannel));
  mDSPContainer[fmodDSP->GetName().ToHash()] = dynamic_cast<FMOD_DSP*>(fmodDSP)->GetFMODDSP();
  return fmodDSP;
}

/**
 * @brief Get DSP from channel group
 * @param aGroupName
 * @param aIndex
 * @return 
 */
DSP* FMODSoundSystem::GetDSPFromChannelGroup(HashString const &aGroupName, int aIndex)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  
  FMOD::DSP *dsp;
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  group->getDSP(aIndex, &dsp);
  
  DSP* fmodDSP = new FMOD_DSP(dsp, this, aGroupName + Common::IntToString(aIndex));
  mDSPContainer[fmodDSP->GetName().ToHash()] = dynamic_cast<FMOD_DSP*>(fmodDSP)->GetFMODDSP();
  
  return fmodDSP;
}

/**
 * @brief Delete DSP
 * @param aDSP
 * @return True if DSP was successfully deleted
 */
bool FMODSoundSystem::DeleteDSP(DSP *aDSP)
{
  if(mDSPContainer.find(aDSP->GetName().ToHash()) == mDSPContainer.end())
  {
    DebugLogPrint("DSP %s does not exist.", aDSP->GetName().ToCharArray());
    assert(!"DSP does not exist.");
  }
  
  FMOD_RESULT result = mDSPContainer[aDSP->GetName().ToHash()]->release();
  if(result != FMOD_ERR_DSP_INUSE)
  {
    mDSPContainer.erase(aDSP->GetName().ToHash());
    return true;
  }
  else
  {
    DebugLogPrint("DSP %s in use, cannot delete.", aDSP->GetName().ToCharArray());
  }
  return false;
}

/**
 * @brief Add DSP to channel
 * @param aDSP
 * @param aChannel
 * @param aIndex
 */
void FMODSoundSystem::AddDSPToChannel(DSP *aDSP, int const aChannel, int const aIndex)
{
  if(mDSPContainer.find(aDSP->GetName().ToHash()) == mDSPContainer.end())
  {
    DebugLogPrint("DSP %s does not exist.", aDSP->GetName().ToCharArray());
    assert(!"DSP does not exist.");
  }
  
  FMOD::Channel *channel;
  FMOD_RESULT result = mFMODSystem->getChannel(aChannel, &channel);
  if(result == FMOD_OK)
  {
    FMOD::DSP* dsp = mDSPContainer[aDSP->GetName().ToHash()];
    channel->addDSP(aIndex, dsp);
  }
  else
  {
    DebugLogPrint("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD failed to get channel.");
  }
}

/**
 * @brief Add DSP to channel group
 * @param aDSP
 * @param aGroupName
 * @param aIndex
 */
void FMODSoundSystem::AddDSPToChannelGroup(DSP *aDSP, HashString const& aGroupName, int const aIndex)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  if(mDSPContainer.find(aDSP->GetName().ToHash()) == mDSPContainer.end())
  {
    DebugLogPrint("DSP %s does not exist.", aDSP->GetName().ToCharArray());
    assert(!"DSP does not exist.");
  }
  
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  FMOD::DSP* dsp = mDSPContainer[aDSP->GetName().ToHash()];
  group->addDSP(aIndex, dsp);
}

/**
 * @brief Remove DSP from channel
 * @param aDSP
 * @param aChannel
 */
void FMODSoundSystem::RemoveDSPFromChannel(DSP *aDSP, int const aChannel)
{
  FMOD::Channel *channel;
  FMOD_RESULT result = mFMODSystem->getChannel(aChannel, &channel);
  if(result == FMOD_OK)
  {
    channel->removeDSP(mDSPContainer[aDSP->GetName().ToHash()]);
  }
  else
  {
    DebugLogPrint("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD failed to get channel.");
  }
}

/**
 * @brief Remove DSP from channel group
 * @param aDSP
 * @param aGroupName
 */
void FMODSoundSystem::RemoveDSPFromChannelGroup(DSP *aDSP, HashString const& aGroupName)
{
  if(mChannelGroupContainer.find(aGroupName.ToHash()) == mChannelGroupContainer.end())
  {
    DebugLogPrint("Channel Group %s does not exist.", aGroupName.ToCharArray());
    assert(!"Channel Group does not exist.");
  }
  if(mDSPContainer.find(aDSP->GetName().ToHash()) == mDSPContainer.end())
  {
    DebugLogPrint("DSP %s does not exist.", aDSP->GetName().ToCharArray());
    assert(!"DSP does not exist.");
  }
  
  FMOD::ChannelGroup *group = mChannelGroupContainer[aGroupName.ToHash()];
  group->removeDSP(mDSPContainer[aDSP->GetName().ToHash()]);
}

/**
 * @brief Set bus mute state.
 * @param aBusName Name of bus.
 * @param aMute Mute state.
 */
void FMODSoundSystem::SetBusMuteState(HashString const &aBusName, bool const aMute)
{
  // Does nothing
}

/**
 * @brief Set bus pause state.
 * @param aBusName Name of bus.
 * @param aPause Pause state.
 */
void FMODSoundSystem::SetBusPauseState(HashString const &aBusName, bool const aPause)
{
  // Does nothing
}

/**
 * @brief Set bus volume.
 * @param aBusName Name of bus.
 * @param aVolume Volume level.
 */
void FMODSoundSystem::SetBusVolume(HashString const &aBusName, float const aVolume)
{
  // Does nothing
}

/**
 * @brief Stop bus.
 * @param aBusName Name of bus.
 */
void FMODSoundSystem::StopBus(HashString const &aBusName)
{
  // Does nothing
}

/**
 * @brief Fade out bus.
 * @param aBusName Name of bus.
 */
void FMODSoundSystem::FadeBus(HashString const &aBusName)
{
  // Does nothing
}

/**
 * @brief Lock bus.
 * @param aBusName Name of bus.
 */
void FMODSoundSystem::LockBus(HashString const &aBusName)
{
  // Does nothing
}

/**
 * @brief Unlock bus.
 * @param aBusName Name of bus.
 */
void FMODSoundSystem::UnlockBus(HashString const &aBusName)
{
  // Does nothing
}
