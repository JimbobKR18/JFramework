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
private:
  typedef std::unordered_map<int, Component*> ComponentContainer;
  typedef ComponentContainer::iterator ComponentIT;
  typedef ComponentContainer::const_iterator ComponentConstIT;
  
  typedef std::unordered_map<int, GameObject*> GameObjectContainer;
  typedef GameObjectContainer::iterator GameObjectIT;
  typedef GameObjectContainer::const_iterator GameObjectConstIT;

  HashString          mFileName;
  HashString          mName;
  ComponentContainer  mComponents;
  ObjectManager*      mManager;
  
  GameObject*         mParent;
  GameObjectContainer mChildren;
  
public:
  GameObject();
  GameObject(ObjectManager *aManager, HashString const &aFileName);
  GameObject(GameObject const &aGameObject);
  virtual ~GameObject();

  // GETTERS
  HashString              GetName() const;
  HashString              GetFileName() const;
  ObjectManager*          GetManager() const;
  GameObject*             GetParent() const;
  
  // SETTERS
  void                    SetName(HashString const &aName);
  void                    SetParent(GameObject *aParent);
  
  // OPERATIONS
  void                    AddComponent(Component *aComponent);
  void                    RemoveComponent(Component *aComponent, bool aDelete);
  void                    RemoveComponent(int const &aUID, bool aDelete);
  Component*              GetComponent(int const &aUID) const;
  Component*              GetComponentByName(HashString const &aName) const;
  bool                    HasComponent(int const &aUID) const;
  bool                    HasComponentByName(HashString const &aName) const;

  // VIRTUALS / STATICS
  virtual void            Update();
  virtual void            ReceiveMessage(Message const &aMessage);
  virtual void            Serialize(Parser &aParser);
  virtual void            Interact(GameObject *aObject);
  static void             SerializeLUA();
  
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
