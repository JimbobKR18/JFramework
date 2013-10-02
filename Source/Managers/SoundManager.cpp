/* SoundManager.cpp
 * Jimmy Spencer
 * 1/27/13
 *
 * The idea: The implementation of SoundManager, what else?
 */


#include "SoundManager.h"
#if !defined(ANDROID) && !defined(IOS)
#include "PCSound.h"
#endif

SoundManager::SoundManager(GameApp *aApp) : Manager(aApp, "SoundManager")
{
}

SoundManager::~SoundManager()
{
}

Sound* SoundManager::CreateSound(std::string const &aFilename)
{
#if !defined(ANDROID) && !defined(IOS)
  Sound *newSound = new PCSound(aFilename);
#else
  Sound *newSound = new Sound(aFilename);
#endif
  AddSound(newSound);
  return newSound;
}
void SoundManager::DeleteSound(Sound* aSound)
{
  RemoveSound(aSound);
  delete aSound;
}
void SoundManager::PlaySound(std::string const &aFilename)
{
  mSounds[aFilename]->Play();
}
void SoundManager::MixAudio(void *aUnused, unsigned char *aStream, unsigned int aLength)
{
  for(SoundIt it = mSounds.begin(); it != mSounds.end(); ++it)
  {
    it->second->Mix(aUnused, aStream, aLength);
  }
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

void SoundManager::AddSound(Sound *aSound)
{
  mSounds.insert(SoundPair(aSound->mName, aSound));
}
void SoundManager::RemoveSound(Sound *aSound)
{

}
