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

#include "Common.h"
#include "Manager.h"
#include "Sound.h"

class SoundManager : public Manager
{
private:
  typedef std::unordered_map<int, Sound*>           SoundContainer;
  typedef SoundContainer::iterator                  SoundIt;
  SoundContainer                                    mSounds;

  static unsigned const sUID;

public:
  SoundManager(GameApp *aApp);
  ~SoundManager();

  Sound*              CreateSound(HashString const &aFilename);
  void                DeleteSound(Sound* aSound);
  void                AddSound(Sound *aSound);
  void                RemoveSound(Sound *aSound);
  void                PlaySound(HashString const &aFilename, int const aNumLoops);
  void                PlaySoundTimed(HashString const &aFilename, int const aNumLoops, int const aMillis);
  void                StopSound(HashString const &aFilename);
  void                StopSoundTimed(HashString const &aFilename, int const aMillis);
  void                SetVolume(float const aVolume);
  void                SetSoundVolume(HashString const &aFilename, float const aVolume);
  void                ResumeSound(HashString const &aFilename);
  void                PauseSound(HashString const &aFilename);

  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "SoundManager";}
  static unsigned     GetUID() {return sUID;}

  static void         SerializeLUA();
};

#endif
