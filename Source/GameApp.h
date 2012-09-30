#ifndef __JFramework_GameApp_H_
#define __JFramework_GameApp_H_

#include "Common.h"
#include "Manager.h"

class GameApp
{
private:
	std::vector<Manager*> mManagers;
	long mLastFrame;
	float mDT;
public:
	GameApp();
	~GameApp();

	float GetDT() const;

	void AppStep();
	void Update();

	void AddManager(Manager* aManager);

	Manager* GetManager(std::string const &aName);

	template<typename T>
	T* GET() {return GetManager(T::GetName());}
};

extern GameApp *gGameApp;

#endif
