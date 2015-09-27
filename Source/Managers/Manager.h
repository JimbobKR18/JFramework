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
	std::string         mName;
  unsigned            mUID;

public:
	std::list<Message *> mDelayedMessages;

	typedef std::list<Message*>::iterator MessageIT;
	typedef std::list<Message*>::const_iterator ConstMessageIT;

	Manager() {assert(0);}
	Manager(GameApp *aApp, std::string aName, unsigned aUID) : mApp(aApp), mName(aName), mUID(aUID) {}
	virtual ~Manager() {}

	GameApp*            GetOwningApp() {return mApp;}
	std::string         GetDefinedName() {return mName;}
  unsigned            GetDefinedUID() {return mUID;}

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
