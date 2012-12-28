#include "Common.h"
#include "ObjectManager.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "Transform.h"
#include "GraphicsManager.h"
#include "ControllerManager.h"
#include "PlayerController.h"

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
	for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
		(*it)->Update();
}

void ObjectManager::SendMessage(Message const &aMsg)
{
	for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
		(*it)->ReceiveMessage(aMsg);
}

GameObject *ObjectManager::CreateObject(std::string const &aFilename)
{
	TextParser parser(RelativePath(aFilename));
	GameObject *object = new GameObject(aFilename);
	AddObject(object);
	ParseDictionary(object, parser);
	return object;
}

void ObjectManager::ParseObject(GameObject *aObject)
{
	TextParser parser(RelativePath(aObject->GetFilename()));
	ParseDictionary(aObject, parser);
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
	for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		if(*it == aObj)
		{
			mObjects.erase(it);
			break;
		}
	}
}

void ObjectManager::ParseDictionary(GameObject *aObject, Parser &aParser)
{
	if(aParser.Find("PhysicsObject"))
	{
		PhysicsObject *object = GetOwningApp()->GET<PhysicsWorld>()->CreateObject();

		// What shape is our object? Is it affected by gravity?
		// What is the object's mass? Is it static?
		std::string type = aParser.Find("PhysicsObject", "Shape");
		std::string gravity = aParser.Find("PhysicsObject", "Gravity");
		std::string isstatic = aParser.Find("PhysicsObject", "Static");
		object->SetMass(StringToInt(aParser.Find("PhysicsObject", "Mass")));

		// default true
		if(gravity == "false")
		  GetOwningApp()->GET<PhysicsWorld>()->UnregisterGravity(object);

		// default false
		if(isstatic == "true")
		      object->SetStatic(true);

		if(type == "CUBE")
			object->mShape = PhysicsObject::CUBE;
		else if(type == "SPHERE")
			object->mShape = PhysicsObject::SPHERE;
		else
			assert(!"Invalid shape given");

		aObject->AddComponent(object);
	}
	if(aParser.Find("Transform"))
	{
	  // Get Position, Scale, and Size
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
	if(aParser.Find("Focus"))
	{
		std::string isTarget = aParser.Find("Focus", "IsTarget");

		if(isTarget == "true")
		{
			GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(aObject);
		}
	}
  if(aParser.Find("PlayerController"))
  {
    PlayerController *controller = new PlayerController();
    controller->SetTarget(aObject);
    
    GetOwningApp()->GET<ControllerManager>()->AddController(controller);
    aObject->AddComponent(controller);
  }
}

void ObjectManager::ClearObjects()
{
	/* Now belongs to LevelManager.cpp
	for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		delete *it;
	}*/
	mObjects.clear();
}

