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

/**
 * @brief Determines how to sort between two PhysicsObjects
 * @param aObject1
 * @param aObject2
 */
bool SortPredicate(PhysicsObject *object1, PhysicsObject *object2)
{
  // Left aligned
  float x1 = object1->GetOwner()->GET<Transform>()->GetHierarchicalPosition().x -
      (object1->GetBroadSize().x * object1->GetOwner()->GET<Transform>()->GetHierarchicalScale().x);
  float x2 = object2->GetOwner()->GET<Transform>()->GetHierarchicalPosition().x -
      (object2->GetBroadSize().x * object2->GetOwner()->GET<Transform>()->GetHierarchicalScale().x);
  if(x1 < x2)
  {
    return true;
  }

  return false;
}

unsigned const PhysicsWorld::sUID = Common::StringHashFunction("PhysicsWorld");

PhysicsWorld::PhysicsWorld(GameApp *aApp) : Manager(aApp, "PhysicsWorld", PhysicsWorld::sUID), mGravity(Vector3(0, 20000.0f, 0))
{
}

PhysicsWorld::~PhysicsWorld()
{
}

/**
 * @brief Create PhysicsObject, set manager to this.
 */
PhysicsObject *PhysicsWorld::CreateObject()
{
  PhysicsObject *obj = new PhysicsObject(this);
  AddObject(obj);
  return obj;
}

/**
 * @brief Remove and delete PhysicsObject
 * @param aObject
 */
void PhysicsWorld::DeleteObject(PhysicsObject *aObject)
{
  RemoveObject(aObject);
  delete aObject;
}

/**
 * @brief Track object. (Start colliding)
 * @param aObject
 */
void PhysicsWorld::AddObject(PhysicsObject *aObject)
{
  // Check to see if object is in our list
  PhysicsIT objectEnd = mObjects.end();
  for(PhysicsIT it = mObjects.begin(); it != objectEnd; ++it)
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

/**
 * @brief Removes object from being tracked. (Will no longer collide)
 * @param aObject
 */
void PhysicsWorld::RemoveObject(PhysicsObject *aObject)
{
  PhysicsIT objectEnd = mObjects.end();
  for(PhysicsIT it = mObjects.begin(); it != objectEnd; ++it)
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

/**
 * @brief Clear (delete) all PhysicsObjects
 */
void PhysicsWorld::ClearObjects()
{
  PhysicsIT objectEnd = mObjects.end();
  for(PhysicsIT it = mObjects.begin(); it != objectEnd; ++it)
  {
    delete *it;
  }
  mObjects.clear();
  mRegistry.Clear();
}

/**
 * @brief Basic update loop.
 */
void PhysicsWorld::Update()
{
  SortOnAxis();
  SweepAndPrune();
  mRegistry.Update();
  mResolver.Update(GetOwningApp()->GetAppStep());
}

/**
 * @brief Send message to all objects.
 * @param aMessage Message to send.
 */
void PhysicsWorld::SendMessage(Message const &aMessage)
{
  std::vector<PhysicsObject *>::iterator objectEnd = mObjects.end();
  for(std::vector<PhysicsObject *>::iterator it = mObjects.begin(); it != objectEnd; ++it)
  {
    (*it)->ReceiveMessage(aMessage);
  }
}

/**
 * @brief Store message to be read next frame.
 * @param aMessage
 */
void PhysicsWorld::ProcessDelayedMessage(Message *aMessage)
{
}

/**
 * @brief Make this manager visible in LUA.
 */
void PhysicsWorld::SerializeLUA()
{
  SLB::Class<PhysicsWorld>("PhysicsWorld").inherits<Manager>();
}

/**
 * @brief Register force on object (will apply every frame automatically)
 * @param aObject Object to enact force upon every frame.
 * @param aGenerator Force generator.
 */
void PhysicsWorld::RegisterForce(PhysicsObject *aObject, ForceGenerator *aGenerator)
{
  mRegistry.Add(aObject, aGenerator);
}

/**
 * @brief Stop applying a force to an object every frame.
 * @param aObject Object to stop force from affecting.
 * @param aGenerator Force generator.
 */
void PhysicsWorld::UnregisterForce(PhysicsObject *aObject, ForceGenerator *aGenerator)
{
  mRegistry.Remove(aObject, aGenerator);
}

/**
 * @brief Helper method to remove gravity on object.
 * @param aObject
 */
void PhysicsWorld::UnregisterGravity(PhysicsObject *aObject)
{
  UnregisterForce(aObject, &mGravity);
}

/**
 * @brief Checks if line collides with anything in our objects list.
 * @param aLine Line to check collision with
 * @param aIgnoreList Objects to ignore.
 */
bool PhysicsWorld::LineCollidesWithAnything(Line const &aLine, std::vector<PhysicsObject*> const &aIgnoreList)
{
  PhysicsIT end = mObjects.end();
  PhysicsConstIT ignoreBegin = aIgnoreList.begin();
  PhysicsConstIT ignoreEnd = aIgnoreList.end();
  for(PhysicsIT it = mObjects.begin(); it != end; ++it)
  {
    if(std::find(ignoreBegin, ignoreEnd, *it) != ignoreEnd)
      continue;
    PhysicsObject::ShapeIT shapesEnd = (*it)->GetShapes().end();
    for(PhysicsObject::ShapeIT it2 = (*it)->GetShapes().begin(); it2 != shapesEnd; ++it2)
    {
      if(CollisionChecker::CheckLineCollision(aLine, (*it)->GetOwner()->GET<Transform>(), *it2))
        return true;
    }
  }
  return false;
}

/**
 * @brief Helper method to sort objects on axis
 */
void PhysicsWorld::SortOnAxis()
{
  std::sort(mObjects.begin(), mObjects.end(), SortPredicate);
}

/**
 * @brief Helper method to enact sweep and prune broadphase method.
 */
void PhysicsWorld::SweepAndPrune()
{
  PhysicsIT end = mObjects.end();
  for(PhysicsIT it = mObjects.begin(); it != end; ++it)
  {
    PhysicsObject *itObject = *it;
    
    if(!itObject->IsActive())
      continue;
    
    HashString itName = itObject->GetOwner()->GetName();
    Transform *itTransform = itObject->GetOwner()->GET<Transform>();
    float x1 = itTransform->GetHierarchicalPosition().x;
    float x1Size = itObject->GetBroadSize().x * itTransform->GetHierarchicalScale().x;

    for(PhysicsIT it2 = it; it2 != end; ++it2)
    {
      PhysicsObject *it2Object = *it2;
      Transform *it2Transform = it2Object->GetOwner()->GET<Transform>();
      
      if(itObject == it2Object)
        continue;
      else if(!it2Object->IsActive())
        continue;
      else if(itObject->IsStatic() && it2Object->IsStatic())
        continue;
      else if(itObject->IgnoreObject(it2Object->GetOwner()) || it2Object->IgnoreObject(itObject->GetOwner()))
        continue;

      float realDistance = (itTransform->GetHierarchicalPosition() - it2Transform->GetHierarchicalPosition()).length();
      PotentialPair potentialPair(itObject, it2Object, realDistance);
      if(!mResolver.Find(potentialPair))
      {
        float x2 = it2Transform->GetHierarchicalPosition().x;
        float x2Size = it2Object->GetBroadSize().x * it2Transform->GetHierarchicalScale().x;
        float xPosDiff = fabs(x1 - x2);
        float xSizeTotal = x1Size + x2Size;
        
        if(xSizeTotal > xPosDiff)
        {
          mResolver.AddPrelimPair(potentialPair);
        }
        else
        {
          break;
        }
      }
    }
  }
}
