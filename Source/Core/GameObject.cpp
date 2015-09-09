//
//  GameObject.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/2/12.
//
//

#include "LuaIncludes.h"
#include "GameObject.h"
#include "Component.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "Surface.h"

GameObject::GameObject()
{
  assert(!"GameObject instantiated without a file!");
}

GameObject::GameObject(ObjectManager *aManager, std::string const &aFileName) :
                       mFileName(aFileName), mName(""), mComponents(), mManager(aManager)
{
  for(int i = static_cast<int>(aFileName.size()) - 1;
      aFileName[i] != '/' && i >= 0; --i)
  {
    mName.push_back(aFileName[i]);
  }
  std::reverse(mName.begin(), mName.end());
  mName = mName.substr(0, mName.size() - 4);
}

// Sounds like a bad idea right now...
GameObject::GameObject(GameObject const &aGameObject) :
                              mFileName(aGameObject.mFileName),
                              mName(aGameObject.mName),
                              mComponents(aGameObject.mComponents),
                              mManager(NULL)
{
  assert(!"Copying GameObjects is not supported!");
}

GameObject::~GameObject()
{
  for(ComponentIT it = mComponents.begin(); it != mComponents.end(); ++it)
  {
    delete *it;
  }
  mComponents.clear();
}

/**
 * @brief Get name of game object
 * @return Name of object
 */
std::string GameObject::GetName()
{
  return mName;
}

/**
 * @brief  Get file name of object
 * @return Object's file name
 */
std::string GameObject::GetFileName()
{
	return mFileName;
}

/**
 * @brief Get objectmanager for object
 * @return ObjectManager
 */
ObjectManager *GameObject::GetManager()
{
  return mManager;
}

/**
 * @brief Set name of object
 * @param aName New name of object
 */
void GameObject::SetName(std::string const &aName)
{
  mName = aName;
}

/**
 * @brief Add a component to game object
 * @param aComponent Component to add
 */
void GameObject::AddComponent(Component *aComponent)
{
  aComponent->SetOwner(this);
  mComponents.push_back(aComponent);
}

/**
 * @brief Remove a component from gameobject
 * @param aComponent Component to remove
 * @param aDelete True to delete component while you are at it
 */
void GameObject::RemoveComponent(Component *aComponent, bool aDelete)
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    if(aComponent == *it)
    {
      (*it)->SetOwner(nullptr);
      if(aDelete)
        delete *it;
      mComponents.erase(it);
      end = mComponents.end();
      break;
    }
  }
}

/**
 * @brief Remove component by component's uid
 * @param aUID UID of component to remove
 * @param aDelete True to delete component while you are at it
 */
void GameObject::RemoveComponent(int const &aUID, bool aDelete)
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end;)
  {
    if(aUID == (*it)->GetUID())
    {
      (*it)->SetOwner(nullptr);
      if(aDelete)
        delete *it;
      it = mComponents.erase(it);
      end = mComponents.end();
    }
    else
      ++it;
  }
}

/**
 * @brief Get component by uid
 * @param aUID uid of component
 * @return Component or nullptr
 */
Component *GameObject::GetComponent(int const &aUID)
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    if((*it)->GetDefinedUID() == aUID)
      return *it;
  }
  return nullptr;
}

/**
 * @brief Get component by name
 * @param aName Name of component
 * @return Component or nullptr
 */
Component* GameObject::GetComponentByName(std::string const &aName)
{
  int uid = Common::StringHashFunction(aName);
  return GetComponent(uid);
}

/**
 * @brief Same as get component except returns true or false
 * @param aUID uid of component
 * @return True if object has component
 */
bool GameObject::HasComponent(int const &aUID)
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    if((*it)->GetDefinedUID() == aUID)
      return true;
  }
  return false;
}

/**
 * @brief Same as get component by name except returns true or false
 * @param aName Name of component
 * @return True if object has component
 */
bool GameObject::HasComponentByName(std::string const &aName)
{
  int uid = Common::StringHashFunction(aName);
  return HasComponent(uid);
}

/**
 * @brief Updates components
 */
void GameObject::Update()
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    (*it)->Update();
  }
}

/**
 * @brief Recieve messages from other objects
 * @param aMessage Message to process
 */
void GameObject::ReceiveMessage(Message const &aMessage)
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    (*it)->ReceiveMessage(aMessage);
  }
}

/**
 * @brief Output to file
 * @param aParser File to output to
 */
void GameObject::Serialize(Parser &aParser)
{
  std::string object = std::string("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  aParser.Place(object, "File", mFileName);
  aParser.Place(object, "Name", std::string("Literal(") + mName + std::string(")"));

  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    (*it)->Serialize(aParser);
  }
}

/**
 * @brief Make object usable in LUA
 */
void GameObject::SerializeLUA()
{
  SLB::Class<GameObject>("GameObject")
          .set("GetComponent", &GameObject::GetComponentByName)
          .set("HasComponent", &GameObject::HasComponentByName)
          .set("GetTransform", &GameObject::GET<Transform>)
          .set("GetSurface", &GameObject::GET<Surface>)
          .set("GetName", &GameObject::GetName);
}

/**
 * @brief Interact with another game object
 * @param aObject Object to interact with
 */
void GameObject::Interact(GameObject *aObject) 
{ 
  DebugLogPrint("%s collided into %s\n", mName.c_str(), aObject->GetName().c_str()); 
}
