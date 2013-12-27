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
  int flags=MIX_INIT_OGG|MIX_INIT_MOD;
  int initted = Mix_Init(flags);
  if((initted & flags) != flags)
  {
      printf("Mix_Init: %s\n", Mix_GetError());
      assert(!"Could not initialize SDL_Mixer, aborting.");
      // handle error
  }
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1)
  {
      printf("Mix_OpenAudio: %s\n", Mix_GetError());
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
void SoundManager::DeleteSound(Sound* aSound)
{
  RemoveSound(aSound);
  delete aSound;
}

void SoundManager::AddSound(Sound *aSound)
{
  mSounds.insert(SoundPair(aSound->mName, aSound));
}

void SoundManager::RemoveSound(Sound *aSound)
{
  mSounds.erase(aSound->mName);
}

void SoundManager::PlaySound(std::string const &aFilename)
{
  mSounds[aFilename]->Play();
}

void SoundManager::StopSound(std::string const &aFilename)
{
  mSounds[aFilename]->Stop();
}

void SoundManager::Update()
{
}

void SoundManager::SendMessage(Message const &aMessage)
{
}

void SoundManager::ProcessDelayedMessage(Message *aMessage)
{
}

void SoundManager::SerializeLUA()
{
  SLB::Class<SoundManager>("SoundManager").inherits<Manager>()
      .set("PlaySound", &SoundManager::PlaySound);
}
