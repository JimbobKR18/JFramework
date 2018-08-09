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
#include "StateObject.h"

GameObject::GameObject()
{
  assert(!"GameObject instantiated without a file!");
}

GameObject::GameObject(ObjectManager *aManager, unsigned const &aID, HashString const &aFileName) :
                       mID(aID), mFileName(aFileName), mName(""), mTags(), mComponents(), mManager(aManager),
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
                              mID(aGameObject.mID),
                              mFileName(aGameObject.mFileName),
                              mName(aGameObject.mName),
                              mTags(aGameObject.mTags),
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
    if(mParent)
    {
      mParent->AddChild(it->second);
    }
    else
    {
      it->second->SetParent(nullptr);
    }
  }
  mChildren.clear();

  if(mParent)
    mParent->RemoveChild(this);

  mParent = nullptr;
  mTags.clear();
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
 * @brief Get ID of this object.
 * @return ID.
 */
int GameObject::GetID() const
{
  return mID;
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
 * @brief Set ID of object
 * @param aID New ID of object
 */
void GameObject::SetID(int const &aID)
{
  mID = aID;
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
  if(mComponents[aComponent->GetDefinedUID()] != aComponent)
  {
    aComponent->SetOwner(this);
    mComponents[aComponent->GetDefinedUID()] = aComponent;
  }
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
  // Don't add self.
  if(aObject == this)
    return;

  if(aObject->GetParent())
    aObject->GetParent()->RemoveChild(aObject);
    
  aObject->SetParent(this);
  mChildren[aObject->GetID()] = aObject;
}

/**
 * @brief Remove child from map.
 * @param aObject Child.
 */
void GameObject::RemoveChild(GameObject *aObject)
{
  GameObjectIT it = mChildren.find(aObject->GetID());
  if(it != mChildren.end())
  {
    aObject->SetParent(nullptr);
    mChildren.erase(it);
  }
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
 * @brief Add tag to object.
 * @param aTag Tag to add.
 */
void GameObject::AddTag(HashString const &aTag)
{
  mTags[aTag.ToHash()] = aTag;
}

/**
 * @brief Remove tag from object.
 * @param aTag Tag to remove.
 */
void GameObject::RemoveTag(HashString const &aTag)
{
  TagIT it = mTags.find(aTag.ToHash());
  if(it != mTags.end())
  {
    mTags.erase(aTag.ToHash());
  }
}

/**
 * @brief Remove all tags.
 */
void GameObject::ClearTags()
{
  mTags.clear();
}

/**
 * @brief Verify if object has tag.
 * @param aTag Tag to search for.
 * @return True if tag is there.
 */
bool GameObject::HasTag(HashString const &aTag) const
{
  return mTags.find(aTag.ToHash()) != mTags.end();
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
 * @param aNode ParserNode to output to
 */
void GameObject::Serialize(ParserNode *aNode)
{
  aNode->Place("File", mFileName);
  aNode->Place("Name", HashString("Literal(") + mName + HashString(")"));
  
  if(mParent)
    aNode->Place("Parent", mParent->GetName());

  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    it->second->Serialize(aNode);
  }
  
  if(!mTags.empty())
  {
    std::vector<std::string> tags;
    for(TagConstIT it = mTags.begin(); it != mTags.end(); ++it)
    {
      tags.push_back(it->second);
    }
    aNode->Place("Tags", Common::StringVectorToString(tags));
  }
}

/**
 * @brief Interact with another game object
 * @param aObject Object to interact with
 */
void GameObject::Interact(GameObject *aObject) 
{ 
#ifdef _DEBUG
  DebugLogPrint("%s collided into %s\n", mName.ToCharArray(), aObject->GetName().ToCharArray()); 
#endif
}

/**
 * @brief Parse any additional data.
 * @param aRoot Root to gather data from.
 */
void GameObject::ParseAdditionalData(ParserNode *aRoot)
{
#ifdef _DEBUG
  DebugLogPrint("ParseAdditionalData called on GameObject, nothing to do.\n");
#endif
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
          .set("GetStateObject", &GameObject::GET<StateObject>)
          .set("GetName", &GameObject::GetName);
}
