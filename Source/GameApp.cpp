#include "GameApp.h"
#include "Manager.h"
#include "PhysicsWorld.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"
#include "LevelManager.h"
#include "ControllerManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "DebugManager.h"
#include "LUATypes.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#define DT (1.0f/60.0f)
#define _DEBUG

GameApp::GameApp(int aWidth, int aHeight)
{
  mDT = 0;
  mLastFrame = ((float)Common::timeGetTime()) / 1000.0f;

  AddManager(new ObjectManager(this));
  AddManager(new PhysicsWorld(this));
  AddManager(new GraphicsManager(this, aWidth, aHeight));
  AddManager(new LevelManager(this));
  AddManager(new ControllerManager(this));
  AddManager(new InputManager(this));
  AddManager(new SoundManager(this));
#ifdef _DEBUG
  AddManager(new DebugManager(this));
#endif
  
  LUABind::StaticGameApp::mApp = this;
  LUABind::RegisterClasses();

  LUABind::LoadFunction<std::string>("Main.LUA", "main", "");
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
  float currentTime = ((float)Common::timeGetTime()) / 1000.0f;
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

  for(std::vector<Message*>::iterator it = mDelayedMessages.begin(); it != mDelayedMessages.end(); ++it)
  {
    SendMessage(**it);
    delete *it;
  }
  mDelayedMessages.clear();
}

void GameApp::SendMessage(Message const &aMessage)
{
  for(std::vector<Manager*>::iterator it = mManagers.begin(); it != mManagers.end(); ++it)
	{
    (*it)->SendMessage(aMessage);
	}
}

void GameApp::SendMessageDelayed(Message *aMessage)
{
  mDelayedMessages.push_back(aMessage);
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

void GameApp::SerializeLUA()
{
  SLB::Class<GameApp>("GameApp")
          .set("GetManager", &GameApp::GetManager)
          .set("GetLevelManager", &GameApp::GET<LevelManager>)
          .set("GetObjectManager", &GameApp::GET<ObjectManager>);
}
