#include "GameApp.h"
#include "Manager.h"
#include "PhysicsWorld.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"
#include "LevelManager.h"
#include "ControllerManager.h"
#include "InputManager.h"
#include "LUATypes.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#define DT (1.0f/60.0f)

GameApp::GameApp()
{
  mDT = 0;
  mLastFrame = ((float)timeGetTime()) / 1000.0f;

  AddManager(new ObjectManager(this));
  AddManager(new PhysicsWorld(this));
  AddManager(new GraphicsManager(this));
  AddManager(new LevelManager(this));
  AddManager(new ControllerManager(this));
  AddManager(new InputManager(this));
  
  LUABind::StaticGameApp::mApp = this;
  LUABind::RegisterClasses();

  //LUABind::LoadScriptFromFile("BasicFile.LUA");
  LUABind::LoadFunction<std::string>("BasicFile.LUA", "LoadLevel", "BasicLevel.txt");
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
  float currentTime = ((float)timeGetTime()) / 1000.0f;
  mDT += currentTime - mLastFrame;
  mLastFrame = currentTime;
}

void GameApp::Update()
{
  AppStep();

  if(mDT >= DT)
  {
    while(mDT >= DT)
      mDT -= DT;

    for(std::vector<Manager*>::iterator it = mManagers.begin(); it != mManagers.end(); ++it)
    {
      (*it)->Update();
    }

    mDT = 0;
  }
}

void GameApp::SendMessage(Message const &aMessage)
{
  for(std::vector<Manager*>::iterator it = mManagers.begin(); it != mManagers.end(); ++it)
	{
    (*it)->SendMessage(aMessage);
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
