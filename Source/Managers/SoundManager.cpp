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
#endif

unsigned const SoundManager::sUID = Common::StringHashFunction("SoundManager");
SoundManager::SoundManager(GameApp *aApp) : Manager(aApp, "SoundManager", SoundManager::sUID), mSoundSystem(nullptr)
{
#if !defined(ANDROID) && !defined(IOS)
  mSoundSystem = new SDLSoundSystem();
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
 * @brief Play a sound for a set amount of time.
 * @param aName
 * @param aNumLoops
 * @param aMillis Time in millis.
 * @return Channel of sound
 */
int SoundManager::PlaySoundTimed(HashString const &aName, int const aNumLoops, int const aMillis)
{
  return mSoundSystem->PlaySoundTimed(aName, aNumLoops, aMillis);
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
 * @brief Fade out a sound over time
 * @param aChannel
 * @param aMillis Time in millis
 */
void SoundManager::StopSoundTimed(int aChannel, int const aMillis)
{
  mSoundSystem->StopSoundTimed(aChannel, aMillis);
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
 * @brief Does nothing for now.
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
      .set("PlaySoundTime", &SoundManager::PlaySoundTimed)
      .set("StopSound", &SoundManager::StopSound)
      .set("StopSoundTimed", &SoundManager::StopSoundTimed)
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
