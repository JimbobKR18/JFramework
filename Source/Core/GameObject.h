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
  HashString                          mFileName;
  HashString                          mName;
  std::unordered_map<int, Component*> mComponents;
  ObjectManager*                      mManager;
  
  typedef std::unordered_map<int, Component*>::iterator ComponentIT;
public:
  GameObject();
  GameObject(ObjectManager *aManager, HashString const &aFileName);
  GameObject(GameObject const &aGameObject);
  virtual ~GameObject();

  HashString              GetName();
  HashString              GetFileName();
  ObjectManager*          GetManager();

  void                    SetName(HashString const &aName);

  void                    AddComponent(Component *aComponent);
  void                    RemoveComponent(Component *aComponent, bool aDelete);
  void                    RemoveComponent(int const &aUID, bool aDelete);
  Component*              GetComponent(int const &aUID);
  Component*              GetComponentByName(HashString const &aName);
  bool                    HasComponent(int const &aUID);
  bool                    HasComponentByName(HashString const &aName);

  template<typename T> T* GET()
  {
    return (T*)GetComponent(T::GetUID());
  }

  template<typename T> bool HAS()
  {
    return HasComponent(T::GetUID());
  }

  void                    Update();
  void                    ReceiveMessage(Message const &aMessage);
  void                    Serialize(Parser &aParser);
  static void             SerializeLUA();

  // Fallback logic for collisions
  virtual void            Interact(GameObject *aObject);
};

#endif /* defined(__JFramework__GameObject__) */
