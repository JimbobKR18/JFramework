//
//  GameObject.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/2/12.
//
//

#include "GameObject.h"
#include "Component.h"

GameObject::GameObject()
{
}

// Sounds like a bad idea right now...
GameObject::GameObject(GameObject const &aGameObject) : mComponents(aGameObject.mComponents)
{
}

GameObject::~GameObject()
{
  for(std::vector<Component*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
  {
    delete *it;
  }
}
  
void GameObject::AddComponent(Component*aComponent)
{
  mComponents.push_back(aComponent);
}

void GameObject::RemoveComponent(Component *aComponent)
{
  for(std::vector<Component*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
  {
    if(aComponent == *it)
    {
      mComponents.erase(it);
      break;
    }
  }
}

Component *GameObject::GetComponent(std::string const &aName)
{
  for(std::vector<Component*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
  {
    if((*it)->GetName() == aName)
      return *it;
  }
  return NULL;
}

bool GameObject::HasComponent(std::string const &aName)
{
  for(std::vector<Component*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
  {
    if((*it)->GetName() == aName)
      return true;
  }
  return false;
}