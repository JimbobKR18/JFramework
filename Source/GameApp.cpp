#include "GameApp.h"
#include "Manager.h"
#include "PhysicsWorld.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"
#include "LevelManager.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#define DT (1.0f/60.0f)

GameApp::GameApp()
{
  mLastFrame = timeGetTime() / 1000.0f;

  AddManager(new ObjectManager(this));
  AddManager(new PhysicsWorld(this));
  AddManager(new GraphicsManager(this));
  AddManager(new LevelManager(this));

  //GameObject* targetTest = GET<ObjectManager>()->CreateObject("BasicObject.txt");
  //GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(targetTest);
  Level *testLevel = GET<LevelManager>()->CreateLevel("BasicLevel.txt");
  testLevel->Load();
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
  float currentTime = timeGetTime() / 1000.0f;
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
