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
SoundManager::SoundManager(GameApp *aApp) : Manager(aApp, "SoundManager", SoundManager::sUID), mSoundSystem(nullptr), mDSPContainer()
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
}

/**
 * @brief Play a sound until it finishes.
 * @param aName
 * @param aNumLoops
 * @return Channel of sound
 */
int SoundManager::PlaySound(HashString const &aName, int const aNumLoops)
{
  return mSoundSystem->PlaySound(aName, aNumLoops);
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
  HashString const sound = "Sound_";
  HashString curIndex = sound + Common::IntToString(index);
  HashString const fileName = "SoundAliases.txt";
  
  Parser *parser = ParserFactory::CreateInputParser("Sounds", fileName);
  while(parser->Find(curIndex))
  {
    ParserNode *curSound = parser->Find(curIndex);
    SoundSystem::SoundSource source = SoundSystem::SoundSource::DEFAULT;
    float volume = 1.0f;
    
    if(curSound->Find("Source") && curSound->Find("Source")->GetValue() == "Stream")
      source = SoundSystem::SoundSource::STREAM;
    if(curSound->Find("Volume"))
      volume = curSound->Find("Volume")->GetValue().ToFloat();
    
    CreateSound(curSound->Find("File")->GetValue(), curSound->Find("Name")->GetValue(), volume, source);
    ++index;
    curIndex = sound + Common::IntToString(index);
  }
  
  // Clean up.
  delete parser;
}
