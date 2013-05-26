#ifndef __JFramework_Manager_H_
#define __JFramework_Manager_H_

#include "Message.h"
#include "GameApp.h"
#include "LuaIncludes.h"

class Manager
{
private:
	GameApp*    mApp;
	std::string mName;

public:
	Manager() {assert(0);}
	Manager(GameApp *aApp, std::string aName) : mApp(aApp), mName(aName) {}
	virtual ~Manager() {}

	GameApp*            GetOwningApp() {return mApp;}
	std::string         GetDefinedName() {return mName;}

	virtual void        Update() = 0;
	virtual void        SendMessage(Message const &aMessage) = 0;
	static std::string  GetName() {return "";}
	static void         SerializeLUA()
	{
	  SLB::Class<Manager, SLB::Instance::NoCopy>("Manager")
        .set("GetOwningApp", &Manager::GetOwningApp);
	}
};

#endif
