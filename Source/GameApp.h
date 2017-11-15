#ifndef __JFramework_GameApp_H_
#define __JFramework_GameApp_H_

#include "Common.h"
#include "Message.h"
#include "HashString.h"

class Manager;

class GameApp
{
private:
  std::vector<Manager*> mManagers;
  std::vector<Message*> mDelayedMessages;
  unsigned int          mLastFrame;
  unsigned int          mSkipFrames;
  float                 mDT;
  float                 mAppStep;
  float                 mAppSpeed;
  bool                  mActive;
public:
  GameApp();
  virtual ~GameApp();

  float             GetAppStep() const;
  void              SetAppStep(float const aAppStep);
  float             GetAppSpeed() const;
  void              SetAppSpeed(float const aAppSpeed);
  bool              GetActive() const;
  void              SetActive(bool const aActive);
  void              SetLastFrameTime(unsigned int const &aLastFrame);
  void              SkipFrames(unsigned int const &aSkipFrames);
  virtual void      Update(unsigned int const &aTicksSinceStart);
  void              SendMessage(Message const &aMessage);
  void              SendMessageDelayed(Message *aMessage);
  void              ClearDelayedMessages();
  void              Start();
  void              AddManager(Manager* aManager);
  Manager*          GetManagerByName(HashString const &aName);
  Manager*          GetManagerByUID(unsigned const aUID);
  
  virtual void      BetweenFrameUpdate();

  template<typename T>
  T*                GET() {return (T*)GetManagerByUID(T::GetUID());}

  static void       SerializeLUA();
};

#endif
