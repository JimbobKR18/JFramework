#ifndef __JFramework_GameApp_H_
#define __JFramework_GameApp_H_

#include "Common.h"
#include "Message.h"

class Manager;

class GameApp
{
private:
	std::vector<Manager*> mManagers;
	std::vector<Message*> mDelayedMessages;
	float                 mLastFrame;
	float                 mDT;
public:
	GameApp(int aWidth, int aHeight);
	virtual ~GameApp();

	float             GetDT() const;
	void              AppStep();
	virtual void      Update();
  void              SendMessage(Message const &aMessage);
  void              SendMessageDelayed(Message *aMessage);
  void              Start();
	void              AddManager(Manager* aManager);
	Manager*          GetManager(std::string const &aName);

	template<typename T>
	T*                GET() {return (T*)GetManager(T::GetName());}

	static void       SerializeLUA();
};

#endif
