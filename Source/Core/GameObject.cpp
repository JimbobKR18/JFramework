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
#include "PhysicsObject.h"

GameObject::GameObject()
{
  assert(!"GameObject instantiated without a file!");
}

GameObject::GameObject(ObjectManager *aManager, HashString const &aFileName) :
                       mFileName(aFileName), mName(""), mComponents(), mManager(aManager),
                       mParent(nullptr), mChildren()
{
  for(int i = aFileName.Length() - 1; i >= 0 && aFileName[i] != '/'; --i)
  {
    mName.Push(aFileName[i]);
  }
  mName.Reverse();
  mName = mName.SubString(0, mName.Size() - 4);
}

// Sounds like a bad idea right now...
GameObject::GameObject(GameObject const &aGameObject) :
                              mFileName(aGameObject.mFileName),
                              mName(aGameObject.mName),
                              mComponents(aGameObject.mComponents),
                              mManager(nullptr),
                              mParent(nullptr), 
                              mChildren()
{
  assert(!"Copying GameObjects is not supported!");
}

GameObject::~GameObject()
{
  for(ComponentIT it = mComponents.begin(); it != mComponents.end(); ++it)
  {
    delete it->second;
  }
  mComponents.clear();
  
  for(GameObjectIT it = mChildren.begin(); it != mChildren.end(); ++it)
  {
    it->second->SetParent(mParent);
  }
  if(mParent)
  {
    mParent->RemoveChild(this);
  }
  mParent = nullptr;
  mChildren.clear();
}

/**
 * @brief Get parent of this object.
 * @return Parent.
 */
GameObject* GameObject::GetParent() const
{
  return mParent;
}

/**
 * @brief Get name of game object
 * @return Name of object
 */
HashString GameObject::GetName() const
{
  return mName;
}

/**
 * @brief  Get file name of object
 * @return Object's file name
 */
HashString GameObject::GetFileName() const
{
  return mFileName;
}

/**
 * @brief Get objectmanager for object
 * @return ObjectManager
 */
ObjectManager *GameObject::GetManager() const
{
  return mManager;
}

/**
 * @brief Set name of object
 * @param aName New name of object
 */
void GameObject::SetName(HashString const &aName)
{
  mName = aName;
}

/**
 * @brief Set parent of object.
 * @param aParent Parent.
 */
void GameObject::SetParent(GameObject *aParent)
{
  mParent = aParent;
}

/**
 * @brief Get all children for this object.
 * @return Children.
 */
GameObject::GameObjectContainer const& GameObject::GetChildren() const
{
  return mChildren;
}

/**
 * @brief Add a component to game object
 * @param aComponent Component to add
 */
void GameObject::AddComponent(Component *aComponent)
{
  aComponent->SetOwner(this);
  mComponents[aComponent->GetDefinedUID()] = aComponent;
}

/**
 * @brief Remove a component from game object
 * @param aComponent Component to remove
 * @param aDelete True to delete component while you are at it
 */
void GameObject::RemoveComponent(Component *aComponent, bool aDelete)
{
  // If component is nullptr, skip.
  if(!aComponent)
    return;
  RemoveComponent(aComponent->GetDefinedUID(), aDelete);
}

/**
 * @brief Remove component by component's uid
 * @param aUID UID of component to remove
 * @param aDelete True to delete component while you are at it
 */
void GameObject::RemoveComponent(int const &aUID, bool aDelete)
{
  ComponentIT component = mComponents.find(aUID);
  if(component != mComponents.end())
  {
    component->second->SetOwner(nullptr);
    if(aDelete)
      delete component->second;
    mComponents.erase(component);
  }
}

/**
 * @brief Add child to map.
 * @param aObject Child.
 */
void GameObject::AddChild(GameObject* aObject)
{
  if(aObject->GetParent())
    aObject->GetParent()->RemoveChild(aObject);
    
  aObject->SetParent(this);
  mChildren[aObject->GetName().ToHash()] = aObject;
}

/**
 * @brief Remove child from map.
 * @param aObject Child.
 */
void GameObject::RemoveChild(GameObject *aObject)
{
  aObject->SetParent(nullptr);
  mChildren.erase(aObject->GetName().ToHash());
}

/**
 * @brief Get component by uid
 * @param aUID uid of component
 * @return Component or nullptr
 */
Component *GameObject::GetComponent(int const &aUID) const
{
  ComponentConstIT component = mComponents.find(aUID);
  if(component != mComponents.end())
  {
    return component->second;
  }
  return nullptr;
}

/**
 * @brief Get component by name
 * @param aName Name of component
 * @return Component or nullptr
 */
Component* GameObject::GetComponentByName(HashString const &aName) const
{
  return GetComponent(aName.ToHash());
}

/**
 * @brief Same as get component except returns true or false
 * @param aUID uid of component
 * @return True if object has component
 */
bool GameObject::HasComponent(int const &aUID) const
{
  ComponentConstIT component = mComponents.find(aUID);
  if(component != mComponents.end())
  {
    return true;
  }
  return false;
}

/**
 * @brief Same as get component by name except returns true or false
 * @param aName Name of component
 * @return True if object has component
 */
bool GameObject::HasComponentByName(HashString const &aName) const
{
  return HasComponent(aName.ToHash());
}

/**
 * @brief Updates components
 */
void GameObject::Update()
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    it->second->Update();
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
    it->second->ReceiveMessage(aMessage);
  }
}

/**
 * @brief Output to file
 * @param aParser File to output to
 */
void GameObject::Serialize(Parser &aParser)
{
  HashString object = HashString("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  aParser.Place(object, "File", mFileName);
  aParser.Place(object, "Name", HashString("Literal(") + mName + HashString(")"));
  
  if(mParent)
    aParser.Place(object, "Parent", mParent->GetName());

  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    it->second->Serialize(aParser);
  }
}

/**
 * @brief Interact with another game object
 * @param aObject Object to interact with
 */
void GameObject::Interact(GameObject *aObject) 
{ 
  DebugLogPrint("%s collided into %s\n", mName.ToCharArray(), aObject->GetName().ToCharArray()); 
}

/**
 * @brief Parse any additional data.
 * @param aRoot Root to gather data from.
 */
void GameObject::ParseAdditionalData(Root *aRoot)
{
  DebugLogPrint("ParseAdditionalData called on GameObject, nothing to do.");
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
          .set("GetPhysicsObject", &GameObject::GET<PhysicsObject>)
          .set("GetName", &GameObject::GetName);
}
