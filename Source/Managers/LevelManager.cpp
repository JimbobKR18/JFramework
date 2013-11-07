#include "LevelManager.h"
#include "InputManager.h"
#include "LuaIncludes.h"
#include "LevelChangeMessage.h"

LevelManager::LevelManager(GameApp *aApp) : Manager(aApp, "LevelManager"), mActiveLevel(NULL)
{

}

LevelManager::~LevelManager()
{

}

Level *LevelManager::CreateLevel(std::string const &aFilename)
{
	Level *ret = new Level(this, aFilename);
	AddLevel(ret);
	return ret;
}

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

void LevelManager::DeleteLevel(Level *aLevel)
{
	RemoveLevel(aLevel);
	delete aLevel;
}

void LevelManager::LoadLevelDelayed(std::string const &aLevelName, bool aReset)
{
  GetOwningApp()->GET<InputManager>()->DeclineInputs();
  LevelChangeMessage *msg = new LevelChangeMessage(aLevelName, (aReset ? "true" : "false"));
  ProcessDelayedMessage(msg);
}

void LevelManager::LoadLevel(std::string const &aLevelName, bool aReset)
{
	if(mActiveLevel)
		mActiveLevel->Unload();
  
	for(std::vector<Level*>::const_iterator it = mLevels.begin(); it != mLevels.end(); ++it)
	{
		if((*it)->GetName() == aLevelName)
		{
      if(aReset)
      {
        (*it)->Reset();
      }
			(*it)->Load();
			mActiveLevel = *it;
			GetOwningApp()->GET<InputManager>()->AcceptInputs();
			return;
		}
	}

	assert(!"Level name specified not found.");
}

Level *LevelManager::GetActiveLevel()
{
  return mActiveLevel;
}

void LevelManager::SetActiveLevel(Level *aLevel)
{
  mActiveLevel = aLevel;
}

void LevelManager::Update()
{
  if(mActiveLevel)
    mActiveLevel->Update();
  for(MessageIT it = mDelayedMessages.begin(); it != mDelayedMessages.end(); ++it)
  {
    LevelChangeMessage *msg = (LevelChangeMessage*)*it;
    LoadLevel(msg->GetDescription(), (msg->GetContent() == "true" ? true : false));
    delete *it;
  }
  mDelayedMessages.clear();
}

void LevelManager::SendMessage(Message const &aMessage)
{

}

void LevelManager::ProcessDelayedMessage(Message *aMessage)
{
  mDelayedMessages.push_back(aMessage);
}

void LevelManager::SerializeLUA()
{
  SLB::Class<LevelManager>("LevelManager").inherits<Manager>()
          .set("CreateLevel", &LevelManager::CreateLevel)
          .set("LoadLevel", &LevelManager::LoadLevelDelayed)
          .set("GetActiveLevel", &LevelManager::GetActiveLevel);
}

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
