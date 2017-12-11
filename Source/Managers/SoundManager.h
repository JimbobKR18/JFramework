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
#include "SoundSystem.h"

class SoundManager : public Manager
{
private:
  SoundSystem*                                      mSoundSystem;

  static unsigned const sUID;

public:
  static int const INFINITE_LOOPS = -1;
  static int const ONE_LOOP = 0;

  SoundManager(GameApp *aApp);
  ~SoundManager();

  void                CreateSound(HashString const &aFilename, HashString const &aAlias = "");
  void                DeleteSound(HashString const &aName);
  int                 PlaySound(HashString const &aName, int const aNumLoops);
  void                StopSound(int aChannel);
  void                SetVolume(float const aVolume);
  void                SetSoundVolume(int aChannel, float const aVolume);
  void                ResumeSound(int aChannel);
  void                PauseSound(int aChannel);

  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "SoundManager";}
  static unsigned     GetUID() {return sUID;}

  static void         SerializeLUA();
  
private:
  void                LoadSounds();
};

#endif
