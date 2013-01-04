//
//  GameObject.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/2/12.
//
//

#include "GameObject.h"
#include "Component.h"
#include "ObjectManager.h"

GameObject::GameObject()
{
  assert(!"GameObject instantiated without a file!");
}

GameObject::GameObject(ObjectManager *aOwner, std::string const &aFilename) :
                       mFileName(aFilename), mOwner(aOwner)
{
}

// Sounds like a bad idea right now...
GameObject::GameObject(GameObject const &aGameObject) :
                              mFileName(aGameObject.mFileName),
													    mComponents(aGameObject.mComponents)
{
  assert(!"Copying GameObjects is not supported!");
}

GameObject::~GameObject()
{
  for(ComponentIT it = mComponents.begin(); it != mComponents.end(); ++it)
  {
    delete *it;
  }
}

std::string GameObject::GetFilename()
{
	return mFileName;
}

ObjectManager *GameObject::GetOwner()
{
  return mOwner;
}

void GameObject::AddComponent(Component *aComponent)
{
  aComponent->SetOwner(this);
  mComponents.push_back(aComponent);
}

void GameObject::RemoveComponent(Component *aComponent)
{
  for(ComponentIT it = mComponents.begin(); it != mComponents.end(); ++it)
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
  for(ComponentIT it = mComponents.begin(); it != mComponents.end(); ++it)
  {
    if((*it)->GetDefinedName() == aName)
      return *it;
  }
  return NULL;
}

bool GameObject::HasComponent(std::string const &aName)
{
  for(ComponentIT it = mComponents.begin(); it != mComponents.end(); ++it)
  {
    if((*it)->GetDefinedName() == aName)
      return true;
  }
  return false;
}

void GameObject::Update()
{
  for(ComponentIT it = mComponents.begin(); it != mComponents.end(); ++it)
  {
    (*it)->Update();
  }
}

void GameObject::ReceiveMessage(Message const &aMessage)
{
  for(ComponentIT it = mComponents.begin(); it != mComponents.end(); ++it)
  {
    (*it)->ReceiveMessage(aMessage);
  }
}
