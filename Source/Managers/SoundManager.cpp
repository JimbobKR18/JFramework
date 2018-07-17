/* SoundManager.cpp
 * Jimmy Spencer
 * 1/27/13
 *
 * The idea: The implementation of SoundManager, what else?
 */


#include "SoundManager.h"
#include "Common.h"
#include "ParserFactory.h"
#if !defined(ANDROID) && !defined(IOS)
#include "FMODSoundSystem.h"
#endif

unsigned const SoundManager::sUID = Common::StringHashFunction("SoundManager");
SoundManager::SoundManager(GameApp *aApp) : Manager(aApp, "SoundManager", SoundManager::sUID), mSoundSystem(nullptr), 
  mDSPContainer(), mSoundsToDSPs()
{
#if !defined(ANDROID) && !defined(IOS)
  mSoundSystem = new FMODSoundSystem();
#else
  assert(!"Platform not supported. (SoundManager.cpp)");
#endif

  LoadSounds();
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
 * @brief Create a sound by filename.
 * @param aFilename
 * @param aAlias
 * @param aDefaultVolume
 * @param aSource
 */
void SoundManager::CreateSound(HashString const &aFilename, HashString const &aAlias, float const &aDefaultVolume, SoundSystem::SoundSource const &aSource)
{
  mSoundSystem->CreateSound(aFilename, aAlias, aDefaultVolume, aSource);
}

/**
 * @brief Delete a sound.
 * @param aName
 */
void SoundManager::DeleteSound(HashString const &aName)
{
  mSoundSystem->DeleteSound(aName);
  
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
}

/**
 * @brief Play a sound until it finishes.
 * @param aName
 * @param aNumLoops
 * @return Channel of sound
 */
int SoundManager::PlaySound(HashString const &aName, int const aNumLoops)
{
  int channel = mSoundSystem->PlaySound(aName, aNumLoops);
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
 * @brief Create new channel group
 * @param aGroupName
 */
void SoundManager::CreateChannelGroup(HashString const &aGroupName)
{
  mSoundSystem->CreateChannelGroup(aGroupName);
}

/**
 * @brief Add channel to group
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
 * @brief Create new DSP (Digital Signal Processor)
 * @param aName
 * @param aType
 * @return 
 */
DSP* SoundManager::CreateDSP(HashString const &aName, DSP_Type const &aType)
{
  DSP *dsp = mSoundSystem->CreateDSP(aName, aType);
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
    // Delete all uses of dsp.
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
      .set("PauseChannel", &SoundManager::PauseChannel);
}

/**
 * @brief Load sounds from SoundAliases.txt file.
 */
void SoundManager::LoadSounds()
{
  int index = 0;
  HashString const soundStr = "Sound_";
  HashString const dspStr = "DSP_";
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
    dsp->Deserialize(curDsp);
    
    ++index;
    curIndex = dspStr + Common::IntToString(index);
  }
  
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
  
  // Clean up.
  delete parser;
}
