//
//  GameObject.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/2/12.
//
//

#ifndef __JFramework__GameObject__
#define __JFramework__GameObject__

#include "Common.h"
#include "Message.h"
#include "Parser.h"

class Component;
class ObjectManager;

class GameObject
{
public:
  typedef std::unordered_map<int, Component*> ComponentContainer;
  typedef ComponentContainer::iterator ComponentIT;
  typedef ComponentContainer::const_iterator ComponentConstIT;
  
  typedef std::unordered_map<unsigned, GameObject*> GameObjectContainer;
  typedef GameObjectContainer::iterator GameObjectIT;
  typedef GameObjectContainer::const_iterator GameObjectConstIT;
  
  typedef std::unordered_map<int, HashString> TagContainer;
  typedef TagContainer::iterator TagIT;
  typedef TagContainer::const_iterator TagConstIT;
  
private:
  unsigned            mID;
  HashString          mFileName;
  HashString          mName;
  TagContainer        mTags;
  ComponentContainer  mComponents;
  ObjectManager*      mManager;
  
  GameObject*         mParent;
  GameObjectContainer mChildren;
  
public:
  GameObject();
  GameObject(ObjectManager *aManager, unsigned const &aID, HashString const &aFileName);
  GameObject(GameObject const &aGameObject);
  virtual ~GameObject();

  // GETTERS
  int                        GetID() const;
  HashString                 GetName() const;
  HashString                 GetFileName() const;
  ObjectManager*             GetManager() const;
  GameObject*                GetParent() const;
  GameObjectContainer const& GetChildren() const;
  
  // SETTERS
  void                       SetID(int const &aID);
  void                       SetName(HashString const &aName);
  void                       SetParent(GameObject *aParent);
  
  // OPERATIONS
  void                       AddComponent(Component *aComponent);
  void                       RemoveComponent(Component *aComponent, bool aDelete);
  void                       RemoveComponent(int const &aUID, bool aDelete);
  void                       AddChild(GameObject* aObject);
  void                       RemoveChild(GameObject *aObject);
  Component*                 GetComponent(int const &aUID) const;
  Component*                 GetComponentByName(HashString const &aName) const;
  bool                       HasComponent(int const &aUID) const;
  bool                       HasComponentByName(HashString const &aName) const;
  void                       AddTag(HashString const &aTag);
  void                       RemoveTag(HashString const &aTag);
  void                       ClearTags();
  bool                       HasTag(HashString const &aTag) const;

  // VIRTUALS / STATICS
  virtual void               Update();
  virtual void               ReceiveMessage(Message const &aMessage);
  virtual void               Serialize(ParserNode *aNode);
  virtual void               Interact(GameObject *aObject);
  virtual void               ParseAdditionalData(ParserNode *aRoot);
  static void                SerializeLUA();
  
  // TEMPLATES
  template<typename T> T* GET() const
  {
    return (T*)GetComponent(T::GetUID());
  }

  template<typename T> bool HAS() const
  {
    return HasComponent(T::GetUID());
  }
};

#endif /* defined(__JFramework__GameObject__) */
