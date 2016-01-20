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

GameApp::GameApp()
{
  mDT = 0;
  mActive = true;
  mLastFrame = Common::GetNow();
  
  // Autoparses Game/Constants.txt
  Constants::Deserialize();
  
  // Set app refresh rate
  mAppStep = 1.0f / Constants::GetFloat("RefreshRate");

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

/**
 * @brief Get refresh time for app.
 * @return Refresh time for app.
 */
float GameApp::GetAppStep() const
{
  return mAppStep;
}

/**
 * @brief Set refresh time for app.
 * @param aAppStep Refresh time for app.
 */
void GameApp::SetAppStep(float const aAppStep)
{
  mAppStep = aAppStep;
}

/**
 * @brief Get if app is active
 * @return App is active
 */
bool GameApp::GetActive() const
{
  return mActive;
}

/**
 * @brief Set if app is active
 * @param aActive Active state
 */
void GameApp::SetActive(bool const aActive)
{
  mActive = aActive;
}

/**
 * @brief Calculate DT
 */
void GameApp::AppStep()
{
  Common::TimePoint currentTime = Common::GetNow();
  mDT += (float)(TimePointToMilliseconds(currentTime - mLastFrame).count()) / 1000.0f;
  mLastFrame = currentTime;
}

/**
 * @brief Update loop
 */
void GameApp::Update()
{
  AppStep();

  if(mDT >= mAppStep)
  {
    mDT -= mAppStep;

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

/**
 * @brief Send message to managers, right now. Delete or add object calls are not advised in this manner.
 * @param aMessage Message to send
 */
void GameApp::SendMessage(Message const &aMessage)
{
  for(std::vector<Manager*>::iterator it = mManagers.begin(); it != mManagers.end(); ++it)
  {
    (*it)->SendMessage(aMessage);
  }
}

/**
 * @brief Send message to managers next frame.
 * @param aMessage Message to process next frame
 */
void GameApp::SendMessageDelayed(Message *aMessage)
{
  mDelayedMessages.push_back(aMessage);
}

/**
 * @brief Clear out delayed messages queue.
 */
void GameApp::ClearDelayedMessages()
{
  for(std::vector<Message*>::iterator it = mDelayedMessages.begin(); it != mDelayedMessages.end(); ++it)
  {
    delete *it;
  }
  mDelayedMessages.clear();
}

/**
 * @brief Start the app.
 */
void GameApp::Start()
{
  LUABind::LoadFunction<std::string>("Main.LUA", "main", "");
}

/**
 * @brief Add a manager to our app
 * @param aManager Manager to add
 */
void GameApp::AddManager(Manager *aManager)
{
	mManagers.push_back(aManager);
}

/**
 * @brief Get manager by name
 * @param aName Name of manager
 * @return Manager, or nullptr
 */
Manager* GameApp::GetManagerByName(HashString const &aName)
{
  for(std::vector<Manager*>::iterator it = mManagers.begin(); it != mManagers.end(); ++it)
  {
    if(aName == (*it)->GetDefinedName())
      return *it;
  }
  return nullptr;
}

/**
* @brief Get manager by UID
* @param aUID UID of manager
* @return Manager, or nullptr
*/
Manager* GameApp::GetManagerByUID(unsigned const aUID)
{
  for (std::vector<Manager*>::iterator it = mManagers.begin(); it != mManagers.end(); ++it)
  {
    if (aUID == (*it)->GetDefinedUID())
      return *it;
  }
  return nullptr;
}

/**
 * @brief Set up for LUA
 */
void GameApp::SerializeLUA()
{
  SLB::Class<GameApp>("GameApp")
          .set("GetManager", &GameApp::GetManagerByName)
          .set("GetLevelManager", &GameApp::GET<LevelManager>)
          .set("GetObjectManager", &GameApp::GET<ObjectManager>)
          .set("GetSoundManager", &GameApp::GET<SoundManager>)
          .set("GetAppStep", &GameApp::GetAppStep);
}
