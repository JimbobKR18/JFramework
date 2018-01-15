#include "FMODSoundSystem.h"
#include "FMODDSP_Basic.h"
#include "FMODDSP_Echo.h"

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
  for(FMODChannelGroupIt it = mChannelGroupContainer.begin(); it != mChannelGroupContainer.end(); ++it)
  {
    it->second->release();
  }
  for(FMODDSPIt it = mDSPContainer.begin(); it != mDSPContainer.end(); ++it)
  {
    it->second->release();
    delete it->second;
  }
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
 * @brief Create sound
 * @param aFilename
 * @param aAlias
 * @param aSource
 */
void FMODSoundSystem::CreateSound(HashString const& aFilename, HashString const& aAlias, SoundSource const& aSource)
{
  HashString fileName = Common::RelativePath("Sounds", aFilename);
  FMOD::Sound *sound;
  
  FMOD_MODE mode = FMOD_DEFAULT;
  
  if(aSource == SoundSource::STREAM)
    mode = FMOD_CREATESTREAM;
  
  FMOD_RESULT result = mFMODSystem->createSound(fileName.ToCharArray(), mode, 0, &sound);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD failed to create sound.");
  }
  
  if(!aAlias.Empty())
  {
    mSoundContainer[aAlias.ToHash()] = sound;
  }
  else
  {
    mSoundContainer[aFilename.ToHash()] = sound;
  }
}

/**
 * @brief Delete sound
 * @param aName
 */
void FMODSoundSystem::DeleteSound(HashString const& aName)
{
  mSoundContainer[aName.ToHash()]->release();
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
  FMOD_RESULT result = mFMODSystem->playSound(mSoundContainer[aName.ToHash()], 0, false, &channel);
  channel->setLoopCount(aNumLoops);
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
 * @brief Create channel group
 * @param aGroupName
 */
void FMODSoundSystem::CreateChannelGroup(HashString const &aGroupName)
{
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
  
  DSP *dsp = nullptr;
  switch(aType)
  {
  case DSP_TYPE_ECHO:
    dsp = new FMODDSP_Echo(mFMODSystem, this, aName);
    mDSPContainer[aName.ToHash()] = ((FMODDSP_Echo*)dsp)->GetFMODDSP();
    break;
  default:
    assert(!"DSP_Type not supported.");
    break;
  }
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
  
  DSP* fmodDSP = new FMODDSP_Basic(dsp, this, Common::IntToString(aChannel));
  mDSPContainer[fmodDSP->GetName().ToHash()] = ((FMODDSP_Basic*)fmodDSP)->GetFMODDSP();
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
  
  DSP* fmodDSP = new FMODDSP_Basic(dsp, this, aGroupName + Common::IntToString(aIndex));
  mDSPContainer[fmodDSP->GetName().ToHash()] = ((FMODDSP_Basic*)fmodDSP)->GetFMODDSP();
  
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
  group->addDSP(aIndex, mDSPContainer[aDSP->GetName().ToHash()]);
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
