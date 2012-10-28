#include "GameApp.h"
#include "Manager.h"
#include "PhysicsWorld.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"

GameApp *gGameApp = NULL;

GameApp::GameApp()
{
  mLastFrame = timeGetTime();

  AddManager(new ObjectManager(this));
  AddManager(new PhysicsWorld(this));
  AddManager(new GraphicsManager(this));

  GET<ObjectManager>()->CreateObject("../Assets/BasicObject.txt");
}

GameApp::~GameApp()
{
	for(std::vector<Manager*>::iterator it = mManagers.begin(); it != mManagers.end(); ++it)
	{
		delete (*it);
	}

	mManagers.clear();
}

float GameApp::GetDT() const
{
  return mDT;
}

void GameApp::AppStep()
{
  float currentTime = timeGetTime();
  mDT = currentTime - mLastFrame;
  mLastFrame = currentTime;
}

void GameApp::Update()
{
  AppStep();

  for(std::vector<Manager*>::iterator it = mManagers.begin(); it != mManagers.end(); ++it)
  {
    (*it)->Update();
  }
}

void GameApp::AddManager(Manager *aManager)
{
	mManagers.push_back(aManager);
}

Manager* GameApp::GetManager(std::string const &aName)
{
	for(std::vector<Manager*>::iterator it = mManagers.begin(); it != mManagers.end(); ++it)
	{
		if((*it)->GetDefinedName() == aName)
			return *it;
	}
	return NULL;
}
