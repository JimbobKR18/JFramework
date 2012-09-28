#ifndef __JFramework_Surface_h_
#define __JFramework_Surface_h_

#include "Component.h"

class Surface : public Component
{
private:
public:
	Surface();
	virtual ~Surface() {}

	void Update();
	void SendMessage(Message const &aMessage);
	void ReceiveMessage(Message const &aMessage);
};

#endif
