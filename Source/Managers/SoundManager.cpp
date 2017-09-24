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
#include "PCSound.h"
#endif

unsigned const SoundManager::sUID = Common::StringHashFunction("SoundManager");
SoundManager::SoundManager(GameApp *aApp) : Manager(aApp, "SoundManager", SoundManager::sUID), mSounds()
{
#if !defined(ANDROID) && !defined(IOS)
  int flags=MIX_INIT_OGG;
  int initted = Mix_Init(flags);
  if((initted & flags) != flags)
  {
    DebugLogPrint("Mix_Init: %s\n", Mix_GetError());
  }
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
  {
    DebugLogPrint("Mix_OpenAudio: %s\n", Mix_GetError());
    assert(!"Could not open SDL_Mix OpenAudio, aborting.");
  }
#endif

  LoadSounds();
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
 * @param aAlias
 * @return The newly created sound.
 */
Sound* SoundManager::CreateSound(HashString const &aFilename, HashString const &aAlias)
{
#if !defined(ANDROID) && !defined(IOS)
  Sound *newSound = new PCSound(Common::RelativePath("Sounds", aFilename));
#else
  Sound *newSound = new Sound(Common::RelativePath("Sounds", aFilename));
#endif
  if(!aAlias.Empty())
    newSound->SetName(aAlias);
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
  mSounds[aSound->GetName().ToHash()] = aSound;
}

/**
 * @brief Unmanage a sound.
 * @param aSound
 */
void SoundManager::RemoveSound(Sound *aSound)
{
  mSounds.erase(aSound->GetName().ToHash());
}

/**
 * @brief Play a sound until it finishes.
 * @param aFilename
 */
void SoundManager::PlaySound(HashString const &aFilename, int const aNumLoops)
{
  if(mSounds.find(aFilename.ToHash()) == mSounds.end())
    CreateSound(aFilename);
  mSounds[aFilename.ToHash()]->Play(aNumLoops);
}

/**
 * @brief Play a sound for a set amount of time.
 * @param aFilename
 * @param aMillis Time in millis.
 */
void SoundManager::PlaySoundTimed(HashString const &aFilename, int const aNumLoops, int const aMillis)
{
  if(mSounds.find(aFilename.ToHash()) == mSounds.end())
    CreateSound(aFilename);
  mSounds[aFilename.ToHash()]->Play(aNumLoops, aMillis);
}

/**
 * @brief Stop a sound.
 * @param aFilename
 */
void SoundManager::StopSound(HashString const &aFilename)
{
  if(mSounds.find(aFilename.ToHash()) == mSounds.end())
    return;
  mSounds[aFilename.ToHash()]->Stop();
}

/**
 * @brief Fade out a sound over time
 * @param aFilename
 * @param aMillis Time in millis
 */
void SoundManager::StopSoundTimed(HashString const &aFilename, int const aMillis)
{
  if(mSounds.find(aFilename.ToHash()) == mSounds.end())
    return;
  mSounds[aFilename.ToHash()]->FadeOut(aMillis);
}

/**
 * @brief Set volume of all sounds.
 * @param aVolume Volume value from 0 to 1.
 */
void SoundManager::SetVolume(float const aVolume)
{
  for(SoundIt it = mSounds.begin(); it != mSounds.end(); ++it)
  {
    it->second->SetVolume(aVolume);
  }
}

/**
 * @brief Set the volume of a sound.
 * @param aFilename
 * @param aVolume The volume to set the sound to.
 */
void SoundManager::SetSoundVolume(HashString const &aFilename, float const aVolume)
{
  if(mSounds.find(aFilename.ToHash()) == mSounds.end())
    return;
  mSounds[aFilename.ToHash()]->SetVolume(aVolume);
}

/**
 * @brief Resume a sound from pause.
 * @param aFilename
 */
void SoundManager::ResumeSound(HashString const &aFilename)
{
  if(mSounds.find(aFilename.ToHash()) == mSounds.end())
    return;
  mSounds[aFilename.ToHash()]->Resume();
}

/**
 * @brief Pause a sound.
 * @param aFilename
 */
void SoundManager::PauseSound(HashString const &aFilename)
{
  if(mSounds.find(aFilename.ToHash()) == mSounds.end())
    return;
  mSounds[aFilename.ToHash()]->Pause();
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
