#include "LevelManager.h"
#include "InputManager.h"
#include "LuaIncludes.h"
#include "LevelChangeMessage.h"

unsigned const LevelManager::sUID = Common::StringHashFunction("LevelManager");
LevelManager::LevelManager(GameApp *aApp) : Manager(aApp, "LevelManager", LevelManager::sUID), mActiveLevel(nullptr)
{
}

LevelManager::~LevelManager()
{
  for(LevelIT it = mLevels.begin(); it != mLevels.end(); ++it)
  {
    delete *it;
  }
}

/**
 * @brief Create a level from a filename
 * @param aFilename
 * @param aFolderName
 * @return 
 */
Level *LevelManager::CreateLevel(HashString const &aFilename, HashString const &aFolderName)
{
  Level *ret = GetLevel(aFilename);

  if(!ret)
  {
    ret = new Level(this, aFilename, aFolderName, true);
    AddLevel(ret);
  }

  return ret;
}

/**
 * @brief Get a level by its name
 * @param aLevelName
 * @return 
 */
Level *LevelManager::GetLevel(HashString const &aLevelName)
{
  for(LevelIT it = mLevels.begin(); it != mLevels.end(); ++it)
  {
    if((*it)->GetFileName() == aLevelName || (*it)->GetName() == aLevelName)
    {
      return *it;
    }
  }
  return nullptr;
}

/**
 * @brief Delete a level, the right way.
 * @param aLevel
 */
void LevelManager::DeleteLevel(Level *aLevel)
{
  RemoveLevel(aLevel);
  delete aLevel;
}

/**
 * @brief Push back loading a level to the next frame.
 * @param aLevelName
 * @param aReset
 */
void LevelManager::LoadLevelDelayed(HashString const &aLevelName, bool aReset)
{
  GetOwningApp()->GET<InputManager>()->DeclineInputs();
  LevelChangeMessage *msg = new LevelChangeMessage(aLevelName, (aReset ? "true" : "false"));
  ProcessDelayedMessage(msg);
}

/**
 * @brief Load a level immediately.
 * @param aLevelName
 * @param aReset
 * Note that deleting previous level in pre or post reset functions in level will cause crash bug.
 */
void LevelManager::LoadLevel(HashString const &aLevelName, bool aReset)
{
  for(LevelConstIT it = mLevels.begin(); it != mLevels.end(); ++it)
  {
    if((*it)->GetFileName() == aLevelName || (*it)->GetName() == aLevelName)
    {
      Level* prevLevel = mActiveLevel;
      mActiveLevel = *it;
      if(prevLevel)
        prevLevel->Unload(mActiveLevel);
      if(aReset)
        mActiveLevel->ResetLevel();
      mActiveLevel->Load(prevLevel);
      GetOwningApp()->GET<InputManager>()->AcceptInputs();
      return;
    }
  }

  assert(!"Level name specified not found.");
}

/**
 * @brief Get currently running level.
 * @return 
 */
Level *LevelManager::GetActiveLevel()
{
  return mActiveLevel;
}

/**
 * @brief Basic update loop.
 */
void LevelManager::Update()
{
  if(mActiveLevel)
    mActiveLevel->Update();
  
  MessageIT msgEnd = mDelayedMessages.end();
  for(MessageIT it = mDelayedMessages.begin(); it != msgEnd; ++it)
  {
    if((*it)->GetDescription() == "ResetLevel")
    {
      mActiveLevel->ResetLevel();
    }
    else
    {
      LevelChangeMessage *msg = (LevelChangeMessage*)*it;
      LoadLevel(msg->GetDescription(), (msg->GetContent() == "true" ? true : false));
    }
    delete *it;
  }
  mDelayedMessages.clear();
}

/**
 * @brief Relays a message to current active level.
 * @param aMessage
 */
void LevelManager::SendMessage(Message const &aMessage)
{
  if(mActiveLevel)
    mActiveLevel->ReceiveMessage(aMessage);
}

/**
 * @brief Post a message to the delay queue.
 * @param aMessage
 */
void LevelManager::ProcessDelayedMessage(Message *aMessage)
{
  mDelayedMessages.push_back(aMessage);
}

/**
 * @brief Make this manager visible in LUA.
 */
void LevelManager::SerializeLUA()
{
  SLB::Class<LevelManager>("LevelManager").inherits<Manager>()
          .set("CreateLevel", &LevelManager::CreateLevel)
          .set("LoadLevel", &LevelManager::LoadLevelDelayed)
          .set("GetActiveLevel", &LevelManager::GetActiveLevel);
}

/**
 * @brief Add a level to our levels vector.
 * @param aLevel
 */
void LevelManager::AddLevel(Level *aLevel)
{
  // Check to see if object is in our list
  for(LevelIT it = mLevels.begin(); it != mLevels.end(); ++it)
  {
    if(*it == aLevel || (*it)->GetFileName() == aLevel->GetFileName())
    {
      return;
    }
  }
  
  mLevels.insert(aLevel);
}

/**
 * @brief Remove a level from our levels vector.
 * @param aLevel
 */
void LevelManager::RemoveLevel(Level *aLevel)
{
  if(mActiveLevel == aLevel)
    mActiveLevel = nullptr;
    
  for(LevelIT it = mLevels.begin(); it != mLevels.end(); ++it)
  {
    if(*it == aLevel)
    {
      mLevels.erase(it);
      break;
    }
  }
}
