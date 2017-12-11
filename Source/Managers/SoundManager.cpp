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
#include "SDLSoundSystem.h"
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
 */
void SoundManager::CreateSound(HashString const &aFilename, HashString const &aAlias)
{
  mSoundSystem->CreateSound(aFilename, aAlias);
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
 * @brief Stop a sound.
 * @param aChannel
 */
void SoundManager::StopSound(int aChannel)
{
  mSoundSystem->StopSound(aChannel);
}

/**
 * @brief Set volume of all sounds.
 * @param aVolume Volume value from 0 to 1.
 */
void SoundManager::SetVolume(float const aVolume)
{
  mSoundSystem->SetVolume(aVolume);
}

/**
 * @brief Set the volume of a sound.
 * @param aChannel
 * @param aVolume The volume to set the sound to.
 */
void SoundManager::SetSoundVolume(int aChannel, float const aVolume)
{
  mSoundSystem->SetChannelVolume(aChannel, aVolume);
}

/**
 * @brief Resume a sound from pause.
 * @param aChannel
 */
void SoundManager::ResumeSound(int aChannel)
{
  mSoundSystem->ResumeSound(aChannel);
}

/**
 * @brief Pause a sound.
 * @param aChannel
 */
void SoundManager::PauseSound(int aChannel)
{
  mSoundSystem->PauseSound(aChannel);
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
      .set("StopSound", &SoundManager::StopSound)
      .set("SetSoundVolume", &SoundManager::SetSoundVolume)
      .set("ResumeSound", &SoundManager::ResumeSound)
      .set("PauseSound", &SoundManager::PauseSound);
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
    CreateSound(curSound->Find("File")->GetValue(), curSound->Find("Name")->GetValue());
    ++index;
    curIndex = sound + Common::IntToString(index);
  }
}
