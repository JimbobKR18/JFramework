#ifndef __JFramework_Manager_H_
#define __JFramework_Manager_H_

#include "Message.h"

class Manager
{
private:
	std::string mName;

public:
	Manager() {assert(0);}
	Manager(std::string aName) : mName(aName) {}
	virtual ~Manager() {}

	std::string GetName() {return mName;}

	virtual void Update() = 0;
	virtual void SendMessage(Message const &aMessage) = 0;
};

#endif
