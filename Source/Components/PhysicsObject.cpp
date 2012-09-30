//
//  PhysicsObject.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#include "PhysicsObject.h"
#include "Transform.h"
#include "GameApp.h"

PhysicsObject::PhysicsObject(PhysicsWorld *aWorld) : Component("PhysicsObject"), mWorld(aWorld)
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::Update()
{
  Vector3 position = ((Transform*)GetOwner()->GetComponent("Transform"))->GetPosition();

  mVelocity += mForces * gGameApp->GetDT();
  position += mVelocity * gGameApp->GetDT();

  ((Transform*)GetOwner()->GetComponent("Transform"))->SetPosition(position);

  mForces *= 0;
}

void PhysicsObject::SendMessage(Message const &aMessage)
{

}

void PhysicsObject::ReceiveMessage(Message const &aMessage)
{

}
