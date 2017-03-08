#include "Common.h"
#include "ObjectManager.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "ChemistryObject.h"
#include "Transform.h"
#include "GraphicsManager.h"
#include "ControllerManager.h"
#include "DebugManager.h"
#include "LuaIncludes.h"
#include "ObjectDeleteMessage.h"
#include "ObjectCreateMessage.h"

#if !defined(ANDROID) && !defined(IOS)
  #include "PCShaderSurface.h"
  #include "PCSurface.h"
#endif

unsigned const ObjectManager::sUID = Common::StringHashFunction("ObjectManager");
ObjectManager::ObjectManager(GameApp *aApp) : Manager(aApp, "ObjectManager", ObjectManager::sUID)
{
}

ObjectManager::~ObjectManager()
{
  ClearObjects();
}

/**
 * @brief Basic loop for object management
 */
void ObjectManager::Update()
{
  ObjectIT objectEnd = mObjects.end();
  for(ObjectIT it = mObjects.begin(); it != objectEnd; ++it)
  {
    (*it)->Update();
  }

  MessageIT msgEnd = mDelayedMessages.end();
  for(MessageIT it = mDelayedMessages.begin(); it != msgEnd; ++it)
  {
    if((*it)->GetDescription() == OBJECT_DELETE.ToCharArray()) 
    {
      ObjectDeleteMessage *msg = (ObjectDeleteMessage*)*it;
      DeleteObject(msg->mObject);
      delete *it;
    }
    else if((*it)->GetDescription() == OBJECT_CREATE.ToCharArray())
    {
      ObjectCreateMessage *msg = (ObjectCreateMessage*)*it;
      AddObject(msg->mObject);
      delete *it;
    }
  }
  mDelayedMessages.clear();
}

/**
 * @brief Send message to all objects in vector.
 * @param aMsg
 */
void ObjectManager::SendMessage(Message const &aMsg)
{
  // Can't cache the end because objects can be deleted during message handling.
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    (*it)->ReceiveMessage(aMsg);
  }
}

/**
 * @brief Prepare message for reading next frame.
 * @param aMessage
 */
void ObjectManager::ProcessDelayedMessage(Message *aMessage)
{
  mDelayedMessages.push_back(aMessage);

#ifdef _DEBUG
  if(aMessage->GetDescription() == OBJECT_DELETE.ToCharArray()) 
  {
    ObjectDeleteMessage *msg = (ObjectDeleteMessage*)aMessage;
    GetOwningApp()->GET<DebugManager>()->HandleDelete(msg);
  }
  else if(aMessage->GetDescription() == OBJECT_CREATE.ToCharArray()) 
  {
    ObjectCreateMessage *msg = (ObjectCreateMessage*)aMessage;
    GetOwningApp()->GET<DebugManager>()->HandleCreate(msg);
  }
#endif
}

/**
 * @brief Create an object and add to objects vector, used for initializing.
 * @param aFilename
 * @return The newly created object.
 */
GameObject *ObjectManager::CreateObject(HashString const &aFilename, HashString const &aFolder)
{
  TextParser parser(Common::RelativePath(aFolder, aFilename));
  GameObject *object = new GameObject(this, aFilename);
  AddObject(object);
  ParseDictionary(object, parser);
  return object;
}

/**
 * @brief Create an object BUT DO NOT ADD to objects vector, used for messages.
 * @param aFilename
 * @return The newly created object.
 */
GameObject *ObjectManager::CreateObjectNoAdd(HashString const &aFilename, HashString const &aFolder)
{
  TextParser parser(Common::RelativePath(aFolder, aFilename));
  GameObject *object = new GameObject(this, aFilename);
  ParseDictionary(object, parser);
  return object;
}

/**
 * @brief Parse object from file, create components.
 * @param aObject
 */
void ObjectManager::ParseObject(GameObject *aObject)
{
  TextParser parser(Common::RelativePath("Game", aObject->GetFileName()));
  ParseDictionary(aObject, parser);
}

/**
 * @brief Self explanatory, also removes object from object vector.
 * @param aObj
 */
void ObjectManager::DeleteObject(GameObject *aObj)
{
  RemoveObject(aObj);
  delete aObj;
}

/**
 * @brief Add object to objects vector.
 * @param aObj The object to add
 * @param aStatic Is this a static environment object, as in, not designed to be updated?
 */
void ObjectManager::AddObject(GameObject *aObj, bool aStatic)
{
  if(aObj == nullptr)
  {
    DebugLogPrint("Not sure why, but a null object was created. (ObjectManager AddObject)");
  }
  
  // Check to see if object is in our list
  ObjectIT objectsEnd = mObjects.end();
  for(ObjectIT it = mObjects.begin(); it != objectsEnd; ++it)
  {
    if(*it == aObj)
    {
      return;
    }
  }
  ObjectIT staticObjectsEnd = mStaticObjects.end();
  for(ObjectIT it = mStaticObjects.begin(); it != staticObjectsEnd; ++it)
  {
    if(*it == aObj)
    {
      return;
    }
  }
  
  if(!aStatic)
    mObjects.push_back(aObj);
  else
    mStaticObjects.push_back(aObj);
}

/**
 * @brief Remove object from objects array
 * @param aObj Object to remove
 * @param aDelete Set to true to delete object on the way out.
 */
void ObjectManager::RemoveObject(GameObject *aObj)
{
  ObjectIT objectsEnd = mObjects.end();
  for(ObjectIT it = mObjects.begin(); it != objectsEnd; ++it)
  {
    if(*it == aObj)
    {
      mObjects.erase(it);
      return;
    }
  }
  ObjectIT staticObjectsEnd = mStaticObjects.end();
  for(ObjectIT it = mStaticObjects.begin(); it != staticObjectsEnd; ++it)
  {
    if(*it == aObj)
    {
      mStaticObjects.erase(it);
      return;
    }
  }
}

/**
 * @brief Make this object readable in LUA.
 */
void ObjectManager::SerializeLUA()
{
  SLB::Class<ObjectManager>("ObjectManager").inherits<Manager>()
          .set("CreateObject", &ObjectManager::CreateObject)
          .set("DeleteObject", &ObjectManager::DeleteObject)
          .set("AddObject", &ObjectManager::AddObject)
          .set("RemoveObject", &ObjectManager::RemoveObject)
          .set("ClearObjects", &ObjectManager::ClearObjects);
}

/**
 * @brief Helper function to parse a file and make a GameObject.
 * @param aObject
 * @param aParser
 */
void ObjectManager::ParseDictionary(GameObject *aObject, Parser &aParser)
{
  if(aParser.Find("Name"))
  {
    std::string name = aParser.Find("Name", "Value")->GetValue().ToString();
    aObject->SetName(name);
  }
  if(aParser.Find("PhysicsObject"))
  {
    PhysicsObject *object = GetOwningApp()->GET<PhysicsWorld>()->CreateObject();
    aObject->AddComponent(object);
    object->Deserialize(aParser);
  }
  if(aParser.Find("ChemistryObject"))
  {
    ChemistryObject *object = new ChemistryObject();
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
    PCSurface *surface = nullptr;
#else
    Surface *surface = nullptr;
#endif
    if(aParser.Find("Surface", "UIElement") && aParser.Find("Surface", "UIElement")->GetValue().ToBool())
    {
#if !defined(ANDROID) && !defined(IOS)
      surface = (PCSurface*)GetOwningApp()->GET<GraphicsManager>()->CreateUISurface();
#else
      surface = GetOwningApp()->GET<GraphicsManager>()->CreateUISurface();
#endif
    }
    else
    {
#if !defined(ANDROID) && !defined(IOS)
      surface = (PCSurface*)GetOwningApp()->GET<GraphicsManager>()->CreateSurface();
#else
      surface = GetOwningApp()->GET<GraphicsManager>()->CreateSurface();
#endif
    }

    surface->Deserialize(aParser);
    aObject->AddComponent(surface);
  }
  if(aParser.Find("Focus"))
  {
    bool isTarget = aParser.Find("Focus", "IsFocus")->GetValue().ToBool();

    if(isTarget)
    {
      GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(aObject);
    }
  }
}

/**
 * @brief Clean out all vectors, no delete though, so it leaks.
 */
void ObjectManager::ClearObjects()
{
  mObjects.clear();
  mStaticObjects.clear();
}

