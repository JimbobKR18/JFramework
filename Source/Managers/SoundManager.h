/*
 * SoundManager.cpp
 * Jimmy Spencer
 * 1/27/13
 *
 * The idea: A general manager for sounds, what else could it be?
 *           Will be using a std::map I think, makes sense.
 */

#ifndef __JFramework_SoundManager_h_
#define __JFramework_SoundManager_h_

#include "Manager.h"
#include "Sound.h"

class SoundManager : public Manager
{
private:
  typedef std::pair<std::string, Sound*>            SoundPair;
  typedef std::map<std::string, Sound*>::iterator   SoundIt;
  std::map<std::string, Sound*>                     mSounds;

public:
  SoundManager(GameApp *aApp);
  ~SoundManager();

  Sound*              CreateSound(std::string const &aFilename);
  void                DeleteSound(Sound* aSound);
  void                PlaySound(std::string const &aFilename);
  void                MixAudio(void *aUnused, unsigned char *aStream, unsigned int aLength);

  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "SoundManager";}

private:
  void                AddSound(Sound *aSound);
  void                RemoveSound(Sound *aSound);
};

#endif
