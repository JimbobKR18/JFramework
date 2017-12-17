/* SoundManager.cpp
 * Jimmy Spencer
 * 1/27/13
 *
 * The idea: The implementation of SoundManager, what else?
 */


#include "SoundManager.h"
#include "Common.h"
#include "TextParser.h"
#if !defined(ANDROID) && !defined(IOS)
#include "FMODSoundSystem.h"
#endif

unsigned const SoundManager::sUID = Common::StringHashFunction("SoundManager");
SoundManager::SoundManager(GameApp *aApp) : Manager(aApp, "SoundManager", SoundManager::sUID), mSoundSystem(nullptr)
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
}

/**
 * @brief Create a sound by filename.
 * @param aFilename
 * @param aAlias
 * @param aSource
 */
void SoundManager::CreateSound(HashString const &aFilename, HashString const &aAlias, SoundSystem::SoundSource const &aSource)
{
  mSoundSystem->CreateSound(aFilename, aAlias, aSource);
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
 * @brief Create new DSP (Digital Signal Processor)
 * @param aName
 * @param aType
 * @return 
 */
DSP* SoundManager::CreateDSP(HashString const &aName, DSP_Type const &aType)
{
  return mSoundSystem->CreateDSP(aName, aType);
}

/**
 * @brief Get DSP from channel
 * @param aChannel
 * @param aIndex
 * @return 
 */
DSP* SoundManager::GetDSPFromChannel(int aChannel, int aIndex)
{
  return mSoundSystem->GetDSPFromChannel(aChannel, aIndex);
}

/**
 * @brief Get DSP from channel group
 * @param aGroupName
 * @param aIndex
 * @return 
 */
DSP* SoundManager::GetDSPFromChannelGroup(HashString const &aGroupName, int aIndex)
{
  return mSoundSystem->GetDSPFromChannelGroup(aGroupName, aIndex);
}

/**
 * @brief Delete DSP
 * @param aDSP
 */
void SoundManager::DeleteDSP(DSP *aDSP)
{
  mSoundSystem->DeleteDSP(aDSP);
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
  
  TextParser parser(Common::RelativePath("Sounds", fileName));
  while(parser.Find(curIndex))
  {
    Root *curSound = parser.Find(curIndex);
    SoundSystem::SoundSource source = SoundSystem::SoundSource::DEFAULT;
    
    if(curSound->Find("Source") && curSound->Find("Source")->GetValue() == "Stream")
      source = SoundSystem::SoundSource::STREAM;
    
    CreateSound(curSound->Find("File")->GetValue(), curSound->Find("Name")->GetValue(), source);
    ++index;
    curIndex = sound + Common::IntToString(index);
  }
}
