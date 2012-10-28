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

PhysicsObject::PhysicsObject(PhysicsWorld *aWorld) : Component("PhysicsObject"), mWorld(aWorld),
													 mVelocity(0,0,0), mAcceleration(0,0,0), mForces(0,0,0),
													 mMass(0), mInverseMass(0), mDamping(0.95f)
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::Update()
{
  Vector3 position = ((Transform*)GetOwner()->GetComponent("Transform"))->GetPosition();

  float dt = mWorld->GetOwningApp()->GetDT();

  position += mVelocity * dt;

  Vector3 finalAcceleration = mAcceleration;
  finalAcceleration += mForces * mInverseMass;
  mVelocity += finalAcceleration * dt;
  mVelocity *= powf(mDamping, dt);

  GetOwner()->GET<Transform>()->SetPosition(position);

  mForces *= 0;
}

void PhysicsObject::SendMessage(Message const &aMessage)
{

}

void PhysicsObject::ReceiveMessage(Message const &aMessage)
{

}

void PhysicsObject::AddForce(Vector3 const &aForce)
{
  mForces += aForce;
}

void PhysicsObject::SetVelocity(Vector3 const &aVel)
{
  mVelocity = aVel;
}

void PhysicsObject::SetAcceleration(Vector3 const &aAccel)
{
  mAcceleration = aAccel;
}

float PhysicsObject::GetMass() const
{
	return mMass;
}

void PhysicsObject::SetMass(float aMass)
{
	mMass= aMass;
	mInverseMass = 1.0f / mMass;
}
