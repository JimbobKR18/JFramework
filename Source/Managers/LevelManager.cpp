#include "LevelManager.h"

LevelManager::LevelManager()
{

}

LevelManager::~LevelManager()
{

}

Level *LevelManager::CreateLevel(std::string const &aFilename)
{
	Level *ret = new Level(aFilename);
	AddLevel(ret);
	return ret;
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
	for(std::vector<Surface*>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
	{
		if(*it == aLevel)
		{
			mLevels.erase(it);
			break;
		}
	}
}
