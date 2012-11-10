#include "LevelManager.h"

LevelManager::LevelManager(GameApp *aApp) : Manager(aApp, "LevelManager")
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
	for(std::vector<Level*>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
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

void LevelManager::Update()
{

}

void LevelManager::SendMessage(Message const &aMessage)
{

}

void LevelManager::AddLevel(Level *aLevel)
{
	mLevels.push_back(aLevel);
}

void LevelManager::RemoveLevel(Level *aLevel)
{
	for(std::vector<Level*>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
	{
		if(*it == aLevel)
		{
			mLevels.erase(it);
			break;
		}
	}
}
