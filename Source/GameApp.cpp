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
#include "Constants.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#define DT (1.0f/60.0f)

GameApp::GameApp()
{
  mDT = 0;
  mActive = true;
  mLastFrame = Common::GetNow();
  
  // Autoparses Game/Constants.txt
  Constants::Deserialize();

  // All current managers added here
  // You can add your own manager in your derived class from GameApp.
  AddManager(new ObjectManager(this));
  AddManager(new PhysicsWorld(this));
  AddManager(new GraphicsManager(this, Constants::GetFloat("ScreenWidth"), Constants::GetFloat("ScreenHeight")));
  AddManager(new LevelManager(this));
  AddManager(new ControllerManager(this));
  AddManager(new InputManager(this));
  AddManager(new SoundManager(this));
#ifdef _DEBUG
  AddManager(new DebugManager(this));
#endif
  
  // LUABind needs to register all LUA classes.
  // You can bind your own classes in your derived class from GameApp.
  LUABind::StaticGameApp::mApp = this;
  LUABind::RegisterClasses();
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

bool GameApp::GetActive() const
{
  return mActive;
}

void GameApp::SetActive(bool const aActive)
{
  mActive = aActive;
}

void GameApp::AppStep()
{
  Common::TimePoint currentTime = Common::GetNow();
  mDT += (float)(TimePointToMilliseconds(currentTime - mLastFrame).count()) / 1000.0f;
  mLastFrame = currentTime;
}

void GameApp::Update()
{
  AppStep();

  if(mDT >= DT)
  {
    mDT = DT;

    for(std::vector<Manager*>::iterator it = mManagers.begin(); it != mManagers.end(); ++it)
    {
      (*it)->Update();
    }
    
    // Delete objects
    for(std::vector<Message*>::iterator it = mDelayedMessages.begin(); it != mDelayedMessages.end(); ++it)
    {
      SendMessage(**it);
      delete *it;
    }
    mDelayedMessages.clear();
  }
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

void GameApp::Start()
{
  LUABind::LoadFunction<std::string>("Main.LUA", "main", "");
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
          .set("GetObjectManager", &GameApp::GET<ObjectManager>)
          .set("GetSoundManager", &GameApp::GET<SoundManager>);
}
