#include "LevelManager.h"
#include "InputManager.h"
#include "LuaIncludes.h"
#include "LevelChangeMessage.h"
#include "TextParser.h"

unsigned const LevelManager::sUID = Common::StringHashFunction("LevelManager");
LevelManager::LevelManager(GameApp *aApp) : Manager(aApp, "LevelManager", LevelManager::sUID), mActiveLevel(NULL)
{

}

LevelManager::~LevelManager()
{

}

/**
 * @brief Create a level from a filename
 * @param aFilename
 * @return 
 */
Level *LevelManager::CreateLevel(std::string const &aFilename)
{
  Level *ret = GetLevel(aFilename);

  if(!ret)
  {
    ret = new Level(this, aFilename, true);
	  AddLevel(ret);
  }

	return ret;
}

/**
 * @brief Get a level by its name
 * @param aLevelName
 * @return 
 */
Level *LevelManager::GetLevel(std::string const &aLevelName)
{
	for(LevelsIT it = mLevels.begin(); it != mLevels.end(); ++it)
	{
		if((*it)->GetName() == aLevelName)
		{
			return *it;
		}
	}
	return NULL;
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
void LevelManager::LoadLevelDelayed(std::string const &aLevelName, bool aReset)
{
  GetOwningApp()->GET<InputManager>()->DeclineInputs();
  LevelChangeMessage *msg = new LevelChangeMessage(aLevelName, (aReset ? "true" : "false"));
  ProcessDelayedMessage(msg);
}

/**
 * @brief Load a level immediately.
 * @param aLevelName
 * @param aReset
 */
void LevelManager::LoadLevel(std::string const &aLevelName, bool aReset)
{
  for(std::vector<Level*>::const_iterator it = mLevels.begin(); it != mLevels.end(); ++it)
  {
    if((*it)->GetName() == aLevelName)
    {
      Level* prevLevel = mActiveLevel;
      if(mActiveLevel)
        mActiveLevel->Unload(*it);
      if(aReset)
        (*it)->ResetLevel();
      (*it)->Load(prevLevel);
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
 * @brief Set current active level.
 * @param aLevel
 */
void LevelManager::SetActiveLevel(Level *aLevel)
{
  mActiveLevel = aLevel;
}

/**
 * @brief Write out current active level to a file.
 * @param aFolder
 * @param aFileName
 */
void LevelManager::SaveActiveLevelAs(std::string const &aFolder, std::string const &aFileName)
{
  TextParser objectParser(Common::RelativePath(aFolder, aFileName + ".txt"), MODE_OUTPUT);
  TextParser mapParser(Common::RelativePath(aFolder, aFileName + "_Map.txt"), MODE_OUTPUT);
  mActiveLevel->Serialize(objectParser);
  mActiveLevel->SerializeTileMap(mapParser);
  objectParser.Write();
  mapParser.Write();
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
  for(LevelsIT it = mLevels.begin(); it != mLevels.end(); ++it)
	{
		if(*it == aLevel || (*it)->GetName() == aLevel->GetName())
		{
      return;
		}
	}
  
	mLevels.push_back(aLevel);
}

/**
 * @brief Remove a level from our levels vector.
 * @param aLevel
 */
void LevelManager::RemoveLevel(Level *aLevel)
{
	for(LevelsIT it = mLevels.begin(); it != mLevels.end(); ++it)
	{
		if(*it == aLevel)
		{
			mLevels.erase(it);
			break;
		}
	}
}
