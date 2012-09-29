#include "Common.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	ClearObjects();
}

void ObjectManager::Update()
{
	for(std::vector<GameObject*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
		(*it)->Update();
}

void ObjectManager::SendMessage(Message const &aMsg)
{
	for(std::vector<GameObject*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
		(*it)->ReceiveMessage(aMsg);
}

GameObject *ObjectManager::CreateObject(std::string const &aFilename)
{
	GameObject *object = new GameObject();
	AddObject(object);
	return object;
}

void ObjectManager::DeleteObject(GameObject *aObj)
{
	RemoveObject(aObj);
	delete aObj;
}

void ObjectManager::AddObject(GameObject *aObj)
{
	mObjects.push_back(aObj);
}

void ObjectManager::RemoveObject(GameObject *aObj)
{
	for(std::vector<GameObject*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		if(*it == aObj)
			mObjects.erase(it);
	}
}

void ObjectManager::ClearObjects()
{
	for(std::vector<GameObject*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		delete *it;
	}
	mObjects.clear();
}
