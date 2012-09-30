#include "GameApp.h"
#include "PhysicsWorld.h"
#include "GraphicsManager.h"

GameApp *gGameApp = NULL;

GameApp::GameApp()
{
  SDL_Init(SDL_INIT_EVERYTHING);

  mLastFrame = timeGetTime();

	AddManager(new PhysicsWorld());
	AddManager(new GraphicsManager());
}

GameApp::~GameApp()
{
	for(std::vector<Manager*>::iterator it = mManagers.begin(); it != mManagers.end(); ++it)
	{
		delete (*it);
	}

	mManagers.clear();

	SDL_Quit();
}

float GameApp::GetDT() const
{
  return mDT;
}

void GameApp::AppStep()
{
  long currentTime = timeGetTime();
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
		if((*it)->GetName() == aName)
			return *it;
	}
	return NULL;
}
