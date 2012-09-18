//
//  PhysicsObject.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(PhysicsWorld *aWorld) : Component("PhysicsObject"), mWorld(aWorld)
{
	mBody = mWorld->GetWorld()->CreateBody(&mBodyDef);
}

PhysicsObject::~PhysicsObject()
{
	mWorld->GetWorld()->DestroyBody(mBody);
}

void PhysicsObject::Update()
{

}

void PhysicsObject::SendMessage(Message const &aMessage)
{

}

void PhysicsObject::ReceiveMessage(Message const &aMessage)
{

}
