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

std::string GameObject::GetName()
{
  return mName;
}

std::string GameObject::GetFileName()
{
	return mFileName;
}

ObjectManager *GameObject::GetManager()
{
  return mManager;
}

void GameObject::SetName(std::string const &aName)
{
  mName = aName;
}

void GameObject::AddComponent(Component *aComponent)
{
  aComponent->SetOwner(this);
  mComponents.push_back(aComponent);
}

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

void GameObject::RemoveComponent(std::string const &aName, bool aDelete)
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end;)
  {
    if(aName == (*it)->GetName())
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

Component *GameObject::GetComponent(std::string const &aName)
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    if((*it)->GetDefinedName() == aName)
      return *it;
  }
  return NULL;
}

bool GameObject::HasComponent(std::string const &aName)
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    if((*it)->GetDefinedName() == aName)
      return true;
  }
  return false;
}

void GameObject::Update()
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    (*it)->Update();
  }
}

void GameObject::ReceiveMessage(Message const &aMessage)
{
  ComponentIT end = mComponents.end();
  for(ComponentIT it = mComponents.begin(); it != end; ++it)
  {
    (*it)->ReceiveMessage(aMessage);
  }
}

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

void GameObject::SerializeLUA()
{
  SLB::Class<GameObject>("GameObject")
          .set("GetComponent", &GameObject::GetComponent)
          .set("HasComponent", &GameObject::HasComponent)
          .set("GetTransform", &GameObject::GET<Transform>)
          .set("GetSurface", &GameObject::GET<Surface>)
          .set("GetName", &GameObject::GetName);
}

void GameObject::Interact(GameObject *aObject) 
{ 
  DebugLogPrint("%s collided into %s\n", mName.c_str(), aObject->GetName().c_str()); 
}
