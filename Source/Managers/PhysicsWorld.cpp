//
//  PhysicsWorld.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "Transform.h"
#include "CollisionChecker.h"
#include "PotentialPair.h"
#include "Common.h"

bool SortPredicate(PhysicsObject *object1, PhysicsObject *object2)
{
  // Left aligned
  float x1 = object1->GetOwner()->GET<Transform>()->GetPosition().x -
      object1->GetBroadSize().x;
  float x2 = object2->GetOwner()->GET<Transform>()->GetPosition().x -
        object2->GetBroadSize().x;
  if(x1 < x2)
  {
    return true;
  }

  return false;
}

PhysicsWorld::PhysicsWorld(GameApp *aApp) : Manager(aApp, "PhysicsWorld"), mGravity(Vector3(0, 20000.0f, 0))
{
}

PhysicsWorld::~PhysicsWorld()
{
}

PhysicsObject *PhysicsWorld::CreateObject()
{
  PhysicsObject *obj = new PhysicsObject(this);
  AddObject(obj);
  return obj;
}

void PhysicsWorld::DeleteObject(PhysicsObject *aObject)
{
  RemoveObject(aObject);
  delete aObject;
}

void PhysicsWorld::AddObject(PhysicsObject *aObject)
{
  // Check to see if object is in our list
  for(std::vector<PhysicsObject *>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    if(*it == aObject)
    {
      return;
    }
  }
  
	mObjects.push_back(aObject);
  
  // Check if the object needs gravity to be applied
  if(aObject->IsAffectedByGravity() && !aObject->IsStatic())
    RegisterForce(aObject, &mGravity);
}

void PhysicsWorld::RemoveObject(PhysicsObject *aObject)
{
  for(std::vector<PhysicsObject *>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    if(*it == aObject)
    {
      // Remove all forces associated with an object
      mRegistry.RemoveForcesForObject(*it);
      mObjects.erase(it);
      break;
    }
  }
}

void PhysicsWorld::ClearObjects()
{
  for(PhysicsIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    delete *it;
  }
  mObjects.clear();
}

void PhysicsWorld::Update()
{
  SortOnAxis();
  SweepAndPrune();
  mRegistry.Update();
  mResolver.Update(GetOwningApp()->GetDT());
}

void PhysicsWorld::SendMessage(Message const &aMessage)
{
  for(std::vector<PhysicsObject *>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    (*it)->ReceiveMessage(aMessage);
  }
}

void PhysicsWorld::ProcessDelayedMessage(Message *aMessage)
{
}

void PhysicsWorld::RegisterForce(PhysicsObject *aObject, ForceGenerator *aGenerator)
{
  mRegistry.Add(aObject, aGenerator);
}

void PhysicsWorld::UnregisterForce(PhysicsObject *aObject, ForceGenerator *aGenerator)
{
  mRegistry.Remove(aObject, aGenerator);
}

void PhysicsWorld::UnregisterGravity(PhysicsObject *aObject)
{
  UnregisterForce(aObject, &mGravity);
}

bool PhysicsWorld::LineCollidesWithAnything(Line const &aLine, std::vector<PhysicsObject*> const &aIgnoreList)
{
  PhysicsIT end = mObjects.end();
  for(PhysicsIT it = mObjects.begin(); it != end; ++it)
  {
    if(std::find(aIgnoreList.begin(), aIgnoreList.end(), *it) != aIgnoreList.end())
      continue;
    PhysicsObject::shapeIT shapesEnd = (*it)->GetShapes().end();
    for(PhysicsObject::shapeIT it2 = (*it)->GetShapes().begin(); it2 != shapesEnd; ++it2)
    {
      if(CollisionChecker::CheckLineCollision(aLine, (*it)->GetOwner()->GET<Transform>(), *it2))
        return true;
    }
  }
  return false;
}

void PhysicsWorld::SortOnAxis()
{
  std::sort(mObjects.begin(), mObjects.end(), SortPredicate);
}

void PhysicsWorld::SweepAndPrune()
{
  PhysicsIT end = mObjects.end();
  for(PhysicsIT it = mObjects.begin(); it != end; ++it)
  {
    PhysicsObject *itObject = *it;
    std::string itName = itObject->GetOwner()->GetName();
    Transform *itTransform = itObject->GetOwner()->GET<Transform>();

    for(PhysicsIT it2 = it; it2 != end; ++it2)
    {
      PhysicsObject *it2Object = *it2;
      std::string it2Name = it2Object->GetOwner()->GetName();
      bool ignore = false;//itObject->IgnoreObject(it2Name) || it2Object->IgnoreObject(itName);

      if(itObject != it2Object && !ignore)
      {
        if((!itObject->IsStatic() || !it2Object->IsStatic()) &&
           !mResolver.Find(itObject, it2Object))
        {
          float x1 = itTransform->GetPosition().x;
          float x1Size = itObject->GetBroadSize().x;
          float x2 = it2Object->GetOwner()->GET<Transform>()->GetPosition().x;
          float x2Size = it2Object->GetBroadSize().x;

          float xPosDiff = fabs(x1 - x2);
          float xSizeTotal = x1Size + x2Size;

          if(xSizeTotal > xPosDiff)
          {
            mResolver.AddPrelimPair(PotentialPair(itObject, it2Object));
          }
          else
          {
            break;
          }
        }
      }
    }
  }
}
