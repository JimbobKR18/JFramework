#include "Level.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "MathExt.h"
#include "Transform.h"
#include "GraphicsManager.h"
#include <algorithm>
#include <fstream>
#include <iostream>

Level::Level()
{
	assert(0);
}

Level::Level(LevelManager *aManager, std::string const &aFileName) : mFileName(aFileName), mOwner(aManager), mActive(false)
{
	for(int i = aFileName.size()-1; aFileName[i] != '/' && i >= 0; --i)
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
	std::ifstream infile(RelativePath(mFileName).c_str());
	GameObject *object = NULL;

  if(!infile.good())
    std::cout << "Level file " << mFileName << " not found!" << std::endl;

	while(infile.good())
	{
		std::string param;
		infile >> param;

		if(param.length() == 0)
			break;

		if(param == "Transform")
		{
		  if(object)
		  {
		    Transform *transform = object->GET<Transform>();

		    float posX, posY, posZ,
		          scaleX, scaleY, scaleZ,
		          sizeX, sizeY, sizeZ;
		    std::string empty;

		    infile >> empty;
		    infile >> empty;
		    infile >> empty;
		    infile >> posX;
		    infile >> empty;
		    infile >> empty;
        infile >> posY;
        infile >> empty;
        infile >> empty;
        infile >> posZ;
        infile >> empty;
        infile >> empty;
        infile >> scaleX;
        infile >> empty;
        infile >> empty;
        infile >> scaleY;
        infile >> empty;
        infile >> empty;
        infile >> scaleZ;
        infile >> empty;
        infile >> empty;
        infile >> sizeX;
        infile >> empty;
        infile >> empty;
        infile >> sizeY;
        infile >> empty;
        infile >> empty;
        infile >> sizeZ;
        infile >> empty;

        transform->SetPosition(Vector3(posX,posY,posZ));
        transform->SetScale(Vector3(scaleX,scaleY,scaleZ));
        transform->SetSize(Vector3(sizeX,sizeY,sizeZ));
		  }
		}
		else if(param == "Focus")
		{
		  std::string value, empty;

		  if(object)
		  {
		    infile >> empty;
		    infile >> empty;
		    infile >> empty;
		    infile >> value;

		    if(value == "true")
		    {
		      mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(object);
		    }
		  }
		}
		else
		{
		  object = new GameObject(param);
		  mOwner->GetOwningApp()->GET<ObjectManager>()->ParseObject(object);
		  mObjects.push_back(object);
		}
	}
}
