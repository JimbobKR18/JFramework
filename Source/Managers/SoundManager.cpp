/* SoundManager.cpp
 * Jimmy Spencer
 * 1/27/13
 *
 * The idea: The implementation of SoundManager, what else?
 */


#include "SoundManager.h"
#include "Common.h"
#if !defined(ANDROID) && !defined(IOS)
#include "PCSound.h"
#endif

SoundManager::SoundManager(GameApp *aApp) : Manager(aApp, "SoundManager")
{
#if !defined(ANDROID) && !defined(IOS)
  int flags=MIX_INIT_OGG;
  int initted = Mix_Init(flags);
  if((initted & flags) != flags)
  {
      DebugLogPrint("Mix_Init: %s\n", Mix_GetError());
  }
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1)
  {
      DebugLogPrint("Mix_OpenAudio: %s\n", Mix_GetError());
      assert(!"Could not open SDL_Mix OpenAudio, aborting.");
  }
#endif
}

SoundManager::~SoundManager()
{
#if !defined(ANDROID) && !defined(IOS)
  Mix_Quit();
  Mix_CloseAudio();
#endif
}

/**
 * @brief Create a sound by filename.
 * @param aFilename
 * @return The newly created sound.
 */
Sound* SoundManager::CreateSound(std::string const &aFilename)
{
#if !defined(ANDROID) && !defined(IOS)
  Sound *newSound = new PCSound(Common::RelativePath("Sounds", aFilename));
#else
  Sound *newSound = new Sound(Common::RelativePath("Sounds", aFilename));
#endif
  AddSound(newSound);
  return newSound;
}

/**
 * @brief Delete a sound. Unmanage it too.
 * @param aSound
 */
void SoundManager::DeleteSound(Sound* aSound)
{
  RemoveSound(aSound);
  delete aSound;
}

/**
 * @brief Manage a sound.
 * @param aSound
 */
void SoundManager::AddSound(Sound *aSound)
{
  mSounds.insert(SoundPair(aSound->mName, aSound));
}

/**
 * @brief Unmanage a sound.
 * @param aSound
 */
void SoundManager::RemoveSound(Sound *aSound)
{
  mSounds.erase(aSound->mName);
}

/**
 * @brief Play a sound until it finishes.
 * @param aFilename
 */
void SoundManager::PlaySound(std::string const &aFilename)
{
  mSounds[aFilename]->Play();
}

/**
 * @brief Play a sound for a set amount of time.
 * @param aFilename
 * @param aMillis Time in millis.
 */
void SoundManager::PlaySoundTimed(std::string const &aFilename, int const aMillis)
{
  mSounds[aFilename]->Play(aMillis);
}

/**
 * @brief Stop a sound.
 * @param aFilename
 */
void SoundManager::StopSound(std::string const &aFilename)
{
  mSounds[aFilename]->Stop();
}

/**
 * @brief Does nothing for now.
 */
void SoundManager::Update()
{
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
      .set("PlaySoundTime", &SoundManager::PlaySoundTimed);
}
