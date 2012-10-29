#include "Common.h"
#include "ObjectManager.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "Transform.h"
#include "GraphicsManager.h"

#if !defined(ANDROID) && !defined(IOS)
  #include "PCSurface.h"
#endif

ObjectManager::ObjectManager(GameApp *aApp) : Manager(aApp, "ObjectManager")
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
	TextParser parser(RelativePath(aFilename));
	GameObject *object = new GameObject();
	AddObject(object);
	ParseDictionary(object, parser);
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

void ObjectManager::ParseDictionary(GameObject *aObject, Parser &aParser)
{
	if(aParser.Find("PhysicsObject"))
	{
		PhysicsObject *object = GetOwningApp()->GET<PhysicsWorld>()->CreateObject();
		object->SetMass(StringToInt(aParser.Find("PhysicsObject", "Mass")));

		aObject->AddComponent(object);
	}
	if(aParser.Find("Transform"))
	{
		Transform *transform = new Transform();
		transform->SetPosition(Vector3(StringToInt(aParser.Find("Transform", "PositionX")),
				StringToInt(aParser.Find("Transform", "PositionY")),
				StringToInt(aParser.Find("Transform", "PositionZ"))));
		transform->SetScale(Vector3(StringToInt(aParser.Find("Transform", "ScaleX")),
						StringToInt(aParser.Find("Transform", "ScaleY")),
						StringToInt(aParser.Find("Transform", "ScaleZ"))));
		transform->SetSize(Vector3(StringToInt(aParser.Find("Transform", "SizeX")),
								StringToInt(aParser.Find("Transform", "SizeY")),
								StringToInt(aParser.Find("Transform", "SizeZ"))));
		aObject->AddComponent(transform);
	}
	if(aParser.Find("Surface"))
	{
#if !defined(ANDROID) && !defined(IOS)
		PCSurface *surface = (PCSurface*)GetOwningApp()->GET<GraphicsManager>()->CreateSurface();
		std::string imageName = aParser.Find("Surface", "TextureName");
		surface->LoadImage(imageName);
#else
		Surface *surface = GetOwningApp()->GET<GraphicsManager>()->CreateSurface();
#endif
		aObject->AddComponent(surface);
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

