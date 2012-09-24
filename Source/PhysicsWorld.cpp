//
//  PhysicsWorld.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#include "PhysicsWorld.h"
#include "PhysicsObject.h"

PhysicsWorld::PhysicsWorld() : Manager("PhysicsWorld")
{
	mGravity = b2Vec2(0.0f, -10.0f);
	mDoSleep = true;

	mWorld = new b2World(mGravity);
}

PhysicsWorld::~PhysicsWorld()
{
	delete mWorld;
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

b2World *PhysicsWorld::GetWorld()
{
	return mWorld;
}

void PhysicsWorld::Update()
{
	for(std::vector<PhysicsObject *>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		(*it)->Update();
	}
}

void PhysicsWorld::SendMessage(Message const &aMessage)
{
	for(std::vector<PhysicsObject *>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		(*it)->ReceiveMessage(aMessage);
	}
}

void PhysicsWorld::AddObject(PhysicsObject *aObject)
{
	mObjects.push_back(aObject);
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
