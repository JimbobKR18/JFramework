#include "Level.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include <algorithm>
#include <fstream>

Level::Level()
{
	assert(0);
}

Level::Level(LevelManager *aManager, std::string const &aFileName) : mFileName(aFileName), mOwner(aManager), mActive(false)
{
	for(int i = aFileName.size()-1; aFileName[i] != '/'; --i)
	{
		mName.push_back(aFileName[i]);
	}
	std::reverse(mName.begin(), mName.end());

	ParseFile();
}

Level::~Level()
{
	if(mActive)
		mObjects.clear();
	else
		for(std::vector<GameObject*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
			delete *it;
}

std::string Level::GetName()
{
	return mName;
}

void Level::Load()
{
	for(std::vector<GameObject*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		mOwner->GetOwningApp()->GET<ObjectManager>()->AddObject(*it);
	}
	mActive = true;
}

void Level::Unload()
{
	for(std::vector<GameObject*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		mOwner->GetOwningApp()->GET<ObjectManager>()->RemoveObject(*it);
	}
	mActive = false;
}

void Level::ParseFile()
{
	std::ifstream infile(mFileName.c_str());

	while(infile.good())
	{
		std::string filename;
		infile >> filename;

		mObjects.push_back(new GameObject(filename));
	}
}
