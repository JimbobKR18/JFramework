/* SoundManager.cpp
 * Jimmy Spencer
 * 1/27/13
 *
 * The idea: The implementation of SoundManager, what else?
 */


#include "SoundManager.h"
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>

Sound::Sound()
{
  assert(!"Sound initialized without a name!");
}
Sound::Sound(std::string const &aFilename) : mName(aFilename)
{
  // TODO
}
Sound::~Sound()
{
  delete mData;
}

void Sound::Play()
{

}

SoundManager::SoundManager(GameApp *aApp) : Manager(aApp, "SoundManager")
{
}

SoundManager::~SoundManager()
{
}

Sound* SoundManager::CreateSound(std::string const &aFilename)
{
  Sound *newSound = new Sound(aFilename);
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
  unsigned int amount = 0;

  for(SoundIt it = mSounds.begin(); it != mSounds.end(); ++it)
  {
    amount = it->second->mLength - it->second->mPos;
    if(amount > aLength)
      amount = aLength;

    SDL_MixAudio(aStream, &it->second->mData[it->second->mPos], amount, SDL_MIX_MAXVOLUME);
    it->second->mPos += amount;
  }
}

void SoundManager::Update()
{

}
void SoundManager::SendMessage(Message const &aMessage)
{

}

void SoundManager::AddSound(Sound *aSound)
{
  mSounds.insert(SoundPair(aSound->mName, aSound));
}
void SoundManager::RemoveSound(Sound *aSound)
{

}
