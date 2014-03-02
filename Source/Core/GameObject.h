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
  std::string             mFileName;
  std::string             mName;
  std::vector<Component*> mComponents;
  ObjectManager*          mManager;
  
  typedef std::vector<Component*>::iterator ComponentIT;
public:
  GameObject();
  GameObject(ObjectManager *aManager, std::string const &aFilename);
  GameObject(GameObject const &aGameObject);
  virtual ~GameObject();

  std::string             GetName();
  std::string             GetFilename();
  ObjectManager*          GetManager();

  void                    SetName(std::string const &aName);

  void                    AddComponent(Component *aComponent);
  void                    RemoveComponent(Component *aComponent, bool aDelete);
  void                    RemoveComponent(std::string const &aName, bool aDelete);
  Component*              GetComponent(std::string const &aName);
  bool                    HasComponent(std::string const &aName);

  template<typename T> T* GET()
  {
    return (T*)GetComponent(T::GetName());
  }

  template<typename T> bool HAS()
  {
    return HasComponent(T::GetName());
  }

  void                    Update();
  void                    ReceiveMessage(Message const &aMessage);
  void                    Serialize(Parser &aParser);
  static void             SerializeLUA();

  // Fallback logic for collisions
  virtual void            Interact(GameObject *aObject) {}
};

#endif /* defined(__JFramework__GameObject__) */
