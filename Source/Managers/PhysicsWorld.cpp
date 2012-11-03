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
#include <algorithm>

#define X_LIMIT 10.0f

bool SortPredicate(PhysicsObject *object1, PhysicsObject *object2)
{
	if(object1->GetOwner()->GET<Transform>()->GetPosition().x <
	   object2->GetOwner()->GET<Transform>()->GetPosition().x)
	{
		return true;
	}

	return false;
}

PhysicsWorld::PhysicsWorld(GameApp *aApp) : Manager(aApp, "PhysicsWorld"), mGravity(Vector3(0, -20.0f, 0))
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

void PhysicsWorld::ClearObjects()
{
	for(std::vector<PhysicsObject*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		delete *it;
	}
	mObjects.clear();
}

void PhysicsWorld::Update()
{
	mRegistry.Update();
}

void PhysicsWorld::SendMessage(Message const &aMessage)
{
	for(std::vector<PhysicsObject *>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		(*it)->ReceiveMessage(aMessage);
	}
}

void PhysicsWorld::RegisterForce(PhysicsObject *aObject, ForceGenerator *aGenerator)
{
	mRegistry.Add(aObject, aGenerator);
}

void PhysicsWorld::UnregisterForce(PhysicsObject *aObject, ForceGenerator *aGenerator)
{
	mRegistry.Remove(aObject, aGenerator);
}

void PhysicsWorld::AddObject(PhysicsObject *aObject)
{
	mObjects.push_back(aObject);
	RegisterForce(aObject, &mGravity);
}

void PhysicsWorld::RemoveObject(PhysicsObject *aObject)
{
	for(std::vector<PhysicsObject *>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		if(*it == aObject)
		{
			mObjects.erase(it);
			break;
		}
	}
}

std::vector<PhysicsObject*> PhysicsWorld::SortOnAxis()
{
	std::vector<PhysicsObject*> ret = mObjects;
	std::sort(ret.begin(), ret.end(), SortPredicate);
	return ret;
}

void PhysicsWorld::SweepAndPrune(std::vector<PhysicsObject*> aSortedObjects)
{
	for(std::vector<PhysicsObject*>::iterator it = aSortedObjects.begin();
		it != aSortedObjects.end(); ++it)
	{
		for(std::vector<PhysicsObject*>::iterator it2 = aSortedObjects.begin();
				it2 != aSortedObjects.end(); ++it2)
		{
			if(*it != *it2)
			{
				if(!mResolver.Find(*it, *it2))
				{
					// Check ranges blah blah
				}
			}
		}
	}
}