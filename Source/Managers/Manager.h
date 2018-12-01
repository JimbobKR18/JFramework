#ifndef __JFramework_Manager_H_
#define __JFramework_Manager_H_

#include "Common.h"
#include "Message.h"
#include "GameApp.h"
#include "LuaIncludes.h"

#define DEFAULT_MANAGER_UID 0

class Manager
{
private:
  GameApp*            mApp;
  HashString          mName;
  unsigned            mUID;

public:
  std::list<Message*> mDelayedMessages;

  typedef std::list<Message*>::iterator MessageIT;
  typedef std::list<Message*>::const_iterator ConstMessageIT;

  Manager() {assert(0);}
  Manager(GameApp *aApp, HashString const &aName, unsigned aUID) : mApp(aApp), mName(aName), mUID(aUID) {}
  virtual ~Manager() {}

  GameApp*            GetOwningApp() {return mApp;}
  HashString          GetDefinedName() {return mName;}
  unsigned            GetDefinedUID() {return mUID;}
  
  void                ClearMessages() 
  {
    MessageIT msgEnd = mDelayedMessages.end();
    for(MessageIT it = mDelayedMessages.begin(); it != msgEnd; ++it)
    {
      delete *it;
    }
    mDelayedMessages.clear();
  }

  virtual void        Update() = 0;
  virtual void        SendMessage(Message const &aMessage) = 0;
  virtual void        ProcessDelayedMessage(Message *aMessage) = 0;
  static unsigned     GetUID() {return DEFAULT_MANAGER_UID;}
  static void         SerializeLUA()
  {
    SLB::Class<Manager, SLB::Instance::NoCopy>("Manager")
      .set("GetOwningApp", &Manager::GetOwningApp);
  }
};

#endif
