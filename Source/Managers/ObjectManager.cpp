#include "Common.h"
#include "ObjectManager.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "Transform.h"
#include "GraphicsManager.h"
#include "ControllerManager.h"
#include "PlayerController.h"
#include "LuaIncludes.h"
#include "ObjectDeleteMessage.h"

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
  ObjectIT end = mObjects.end();
	for(ObjectIT it = mObjects.begin(); it != end; ++it)
		(*it)->Update();

	for(MessageIT it = mDelayedMessages.begin(); it != mDelayedMessages.end(); ++it)
  {
    ObjectDeleteMessage *msg = (ObjectDeleteMessage*)*it;
    DeleteObject(msg->mObject);
    delete *it;
  }
	mDelayedMessages.clear();
}

void ObjectManager::SendMessage(Message const &aMsg)
{
  ObjectIT end = mObjects.end();
	for(ObjectIT it = mObjects.begin(); it != end; ++it)
  {
		(*it)->ReceiveMessage(aMsg);
  }
}

void ObjectManager::ProcessDelayedMessage(Message *aMessage)
{
  mDelayedMessages.push_back(aMessage);
}

GameObject *ObjectManager::CreateObject(std::string const &aFilename)
{
	TextParser parser(Common::RelativePath("Game", aFilename));
	GameObject *object = new GameObject(this, aFilename);
	AddObject(object);
	ParseDictionary(object, parser);
	return object;
}

void ObjectManager::ParseObject(GameObject *aObject)
{
	TextParser parser(Common::RelativePath("Game", aObject->GetFilename()));
	ParseDictionary(aObject, parser);
}

void ObjectManager::DeleteObject(GameObject *aObj)
{
	RemoveObject(aObj);
	delete aObj;
}

void ObjectManager::AddObject(GameObject *aObj)
{
  // Check to see if object is in our list
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		if(*it == aObj)
		{
      return;
		}
	}
  
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

void ObjectManager::SerializeLUA()
{
  SLB::Class<ObjectManager>("ObjectManager").inherits<Manager>()
          .set("CreateObject", &ObjectManager::CreateObject);
}

void ObjectManager::ParseDictionary(GameObject *aObject, Parser &aParser)
{
  if(aParser.Find("Name"))
  {
    std::string name = aParser.Find("Name", "Value");
    aObject->SetName(name);
  }
	if(aParser.Find("PhysicsObject"))
	{
		PhysicsObject *object = GetOwningApp()->GET<PhysicsWorld>()->CreateObject();
		aObject->AddComponent(object);
    object->Deserialize(aParser);
	}
	if(aParser.Find("Transform"))
	{
	  // Get Position, Scale, and Size
		Transform *transform = new Transform();
    transform->Deserialize(aParser);
		aObject->AddComponent(transform);
	}
	if(aParser.Find("Surface"))
	{
#if !defined(ANDROID) && !defined(IOS)
		PCSurface *surface = (PCSurface*)GetOwningApp()->GET<GraphicsManager>()->CreateSurface();
#else
		Surface *surface = GetOwningApp()->GET<GraphicsManager>()->CreateSurface();
#endif
    surface->Deserialize(aParser);
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

