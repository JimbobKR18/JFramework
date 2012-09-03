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

class Component;

class GameObject
{
private:
  std::vector<Component*> mComponents;
public:
  GameObject();
  GameObject(GameObject const &aGameObject);
  ~GameObject();
  
  void AddComponent(Component *aComponent);
  void RemoveComponent(Component *aComponent);

  Component *GetComponent(std::string const &aName);
  bool HasComponent(std::string const &aName);

  template<typename T>
  T *GET()
  {
    return (T*)GetComponent(T::GetName());
  }

  template<typename T>
  T *HAS()
  {
    return HasComponent(T::GetName());
  }

  void Update();
};

#endif /* defined(__JFramework__GameObject__) */
