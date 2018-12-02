#include "Common.h"
#include "ObjectManager.h"
#include "EffectsManager.h"
#include "DebugManager.h"
#include "LuaIncludes.h"
#include "ObjectDeleteMessage.h"
#include "ObjectCreateMessage.h"
#include "DefaultGameObjectFactory.h"
#include "ParserFactory.h"

unsigned const ObjectManager::sUID = Common::StringHashFunction("ObjectManager");
ObjectManager::ObjectManager(GameApp *aApp) : Manager(aApp, "ObjectManager", ObjectManager::sUID),
  mArchetypes(), mObjects(), mStaticObjects(), mAllocatedObjects(), mFactory(new DefaultGameObjectFactory())
{
}

ObjectManager::~ObjectManager()
{
  // Clean all objects
  for(ObjectIT it = mAllocatedObjects.begin(); it != mAllocatedObjects.end();)
  {
    DeleteObject(*it);
    it = mAllocatedObjects.begin();
  }
  delete mFactory;
}

GameObjectFactory* ObjectManager::GetGameObjectFactory() const
{
  return mFactory;
}

std::unordered_set<GameObject*>& ObjectManager::GetAllocatedObjects()
{
  return mAllocatedObjects;
}
  
void ObjectManager::SetGameObjectFactory(GameObjectFactory* aFactory)
{
  if(mFactory)
    delete mFactory;
  mFactory = aFactory;
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
  // Avoid double delete.
  for(MessageIT it = mDelayedMessages.begin(); it != mDelayedMessages.end(); ++it)
  {
    Message *message = *it;
    if(message->GetDescription() == OBJECT_DELETE && aMessage->GetDescription() == OBJECT_DELETE)
    {
      ObjectDeleteMessage *oldDeleteMessage = (ObjectDeleteMessage*)message;
      ObjectDeleteMessage *newDeleteMessage = (ObjectDeleteMessage*)aMessage;
      if(oldDeleteMessage->mObject == newDeleteMessage->mObject)
      {
        DebugLogPrint("Double delete called on object in same frame.\n");
        delete aMessage;
        return;
      }
    }
  }
  mDelayedMessages.push_back(aMessage);

#ifdef _DEBUG
  if(aMessage->GetDescription() == OBJECT_DELETE) 
  {
    ObjectDeleteMessage *msg = (ObjectDeleteMessage*)aMessage;
    GetOwningApp()->GET<DebugManager>()->HandleDelete(msg);
  }
  else if(aMessage->GetDescription() == OBJECT_CREATE) 
  {
    ObjectCreateMessage *msg = (ObjectCreateMessage*)aMessage;
    GetOwningApp()->GET<DebugManager>()->HandleCreate(msg);
  }
  
  for(MessageIT it = mDelayedMessages.begin(); it != mDelayedMessages.end(); ++it)
  {
    Message *message = *it;
    if(message->GetDescription() == OBJECT_CREATE && aMessage->GetDescription() == OBJECT_DELETE)
    {
      ObjectCreateMessage *oldCreateMessage = (ObjectCreateMessage*)message;
      ObjectDeleteMessage *newDeleteMessage = (ObjectDeleteMessage*)aMessage;
      if(oldCreateMessage->mObject == newDeleteMessage->mObject)
      {
        DebugLogPrint("Object with name %s being created and deleted in the same frame.\n", oldCreateMessage->mObject->GetName().ToCharArray());
        assert(!"Cannot create and delete a delayed object in the same frame.");
      }
    }
  }
#endif
}

/**
 * @brief Create an object and add to objects vector, used for initializing.
 * @param aFilename
 * @return The newly created object.
 */
GameObject *ObjectManager::CreateObject(HashString const &aFilename, HashString const &aFolder, HashString const &aType)
{
  Parser *parser = ParserFactory::CreateInputParser(aFolder, aFilename);
  GameObject *object = mFactory->CreateGameObject(this, aFilename, aType);
  AddObject(object);
  ParseDictionary(object, parser);
  mAllocatedObjects.insert(object);
  return object;
}

/**
 * @brief Create an object BUT DO NOT ADD to objects vector, used for messages.
 * @param aFilename
 * @return The newly created object.
 */
GameObject *ObjectManager::CreateObjectNoAdd(HashString const &aFilename, HashString const &aFolder, HashString const &aType)
{
  Parser *parser = ParserFactory::CreateInputParser(aFolder, aFilename);
  GameObject *object = mFactory->CreateGameObject(this, aFilename, aType);
  ParseDictionary(object, parser);
  mAllocatedObjects.insert(object);
  return object;
}

/**
 * @brief Find and return first instance of object with name (filename or otherwise)
 * @param aName Name of object (filename or assigned name)
 * @return Object if found, otherwise null.
 */
GameObject* ObjectManager::FindObject(HashString const &aName)
{
  // Check non static objects first
  ObjectIT objectsEnd = mObjects.end();
  for(ObjectIT it = mObjects.begin(); it != objectsEnd; ++it)
  {
    if((*it)->GetName() == aName || (*it)->GetFileName() == aName)
      return *it;
  }
  
  // If nothing, search static objects
  ObjectIT staticObjectsEnd = mStaticObjects.end();
  for(ObjectIT it = mStaticObjects.begin(); it != staticObjectsEnd; ++it)
  {
    if((*it)->GetName() == aName || (*it)->GetFileName() == aName)
      return *it;
  }
  
  return nullptr;
}

/**
 * @brief Parse object from file, create components.
 * @param aObject
 * @param aFolder
 */
void ObjectManager::ParseObject(GameObject *aObject, HashString const &aFolder)
{
  Parser *parser = ParserFactory::CreateInputParser(aFolder, aObject->GetFileName());
  ParseDictionary(aObject, parser);
}

/**
 * @brief Self explanatory, also removes object from object vector.
 * @param aObj
 */
void ObjectManager::DeleteObject(GameObject *aObj)
{
  RemoveObject(aObj);
  
  mAllocatedObjects.erase(aObj);
  ComponentFactory *factory = GetOwningApp()->GetComponentFactory();
  for(GameObject::ComponentIT it = aObj->GetComponents().begin(); it != aObj->GetComponents().end(); ++it)
  {
    Component *component = it->second;
    factory->DeleteComponent(GetOwningApp(), aObj, component);
  }
  aObj->GetComponents().clear();
  delete aObj;
}

/**
 * @brief Delete all objects currently in game.
 */
void ObjectManager::DeleteObjects()
{
  std::unordered_set<GameObject*> allObjects;
  allObjects.insert(mObjects.begin(), mObjects.end());
  allObjects.insert(mStaticObjects.begin(), mStaticObjects.end());
  
  ObjectIT objectsEnd = allObjects.end();
  for(ObjectIT it = allObjects.begin(); it != objectsEnd; ++it)
  {
    GameObject *object = *it;
    mAllocatedObjects.erase(object);
    
    ComponentFactory *factory = GetOwningApp()->GetComponentFactory();
    for(GameObject::ComponentIT it = object->GetComponents().begin(); it != object->GetComponents().end(); ++it)
    {
      Component *component = it->second;
      factory->DeleteComponent(GetOwningApp(), object, component);
    }
    object->GetComponents().clear();
    delete object;
  }
  
  mObjects.clear();
  mStaticObjects.clear();
  ClearMessages();
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
  
  if(!aStatic)
    mObjects.insert(aObj);
  else
    mStaticObjects.insert(aObj);
}

/**
 * @brief Remove object from objects array
 * @param aObj Object to remove
 * @param aDelete Set to true to delete object on the way out.
 */
void ObjectManager::RemoveObject(GameObject *aObj)
{
  mObjects.erase(aObj);
  mStaticObjects.erase(aObj);
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
          .set("FindObject", &ObjectManager::FindObject);
}

/**
 * @brief Helper function to parse a file and make a GameObject.
 * @param aObject
 * @param aParser
 */
void ObjectManager::ParseDictionary(GameObject *aObject, Parser *aParser)
{
  if(aParser->Find("Name"))
  {
    HashString name = aParser->Find("Name", "Value")->GetValue();
    aObject->SetName(name);
  }
  
  ComponentFactory *factory = GetOwningApp()->GetComponentFactory();
  ParserNodeContainer children = aParser->GetBaseRoot()->GetChildren();
  for(parserNodeIT it = children.begin(); it != children.end(); ++it)
  {
    factory->CreateComponent(GetOwningApp(), aObject, *it);
  }
  
  if(aParser->Find("Effects"))
  {
    ParseEffects(aObject, aParser->Find("Effects"));
  }
  
  // Parse additional stuff if need be.
  ParserNodeContainer untouched = aParser->GetBaseRoot()->GetUntouchedRoots();
  for(parserNodeIT it = untouched.begin(); it != untouched.end(); ++it)
  {
    aObject->ParseAdditionalData(*it);
  }
}

/**
 * @brief Parse effects root.
 * @param aObject Object to apply effects to.
 * @param aEffects Effects root.
 */
void ObjectManager::ParseEffects(GameObject *aObject, ParserNode *aEffects)
{
  EffectsManager* effectsManager = GetOwningApp()->GET<EffectsManager>();
  int curIndex = 0;
  HashString const effectString = "Effect_";
  HashString curEffect = effectString + Common::IntToString(curIndex);
  while(aEffects->Find(curEffect))
  {
    ParserNode* effectRoot = aEffects->Find(curEffect);
    HashString type = effectRoot->Find("Type")->GetValue();
    HashString name = effectRoot->Find("Name")->GetValue();
    float time = effectRoot->Find("Time")->GetValue().ToFloat();
    Effect* effect = effectsManager->CreateEffect(type);
    effect->SetName(name);
    effect->SetObject(aObject);
    effect->SetTime(time);
    effect->Deserialize(effectRoot);
    ++curIndex;
    curEffect = effectString + Common::IntToString(curIndex);
  }
}
