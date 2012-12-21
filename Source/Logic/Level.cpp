#include "Level.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "MathExt.h"
#include "Transform.h"
#include "GraphicsManager.h"
#include "Common.h"

Level::Level()
{
	assert(0);
}

Level::Level(LevelManager *aManager, std::string const &aFileName) : mFileName(aFileName), mOwner(aManager), mActive(false)
{
	for(int i = static_cast<int>(aFileName.size()) - 1; aFileName[i] != '/' && i >= 0; --i)
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
	TextParser parser(RelativePath(mFileName).c_str(), false);
	GameObject *object = NULL;

	while(parser.IsGood())
	{
		std::string param;
		parser.GetNextString(param);

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

        parser.GetNextFloat(posX);
        parser.GetNextFloat(posY);
        parser.GetNextFloat(posZ);
        parser.GetNextFloat(scaleX);
        parser.GetNextFloat(scaleY);
        parser.GetNextFloat(scaleZ);
        parser.GetNextFloat(sizeX);
        parser.GetNextFloat(sizeY);
        parser.GetNextFloat(sizeZ);

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
        parser.GetNextString(empty);
        parser.GetNextString(value);

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
