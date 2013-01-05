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
						     mBroadSize(0,0,0), mMass(0), mInverseMass(0), 
						     mDamping(0.01f), mStatic(false), mGravity(true), mShape(SPHERE)
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::Update()
{
  // Store position for later
  Vector3 position = ((Transform*)GetOwner()->GetComponent("Transform"))->GetPosition();

  float dt = mWorld->GetOwningApp()->GetDT();

  position += mVelocity * dt;

  // Get acceleration, adjust velocity
  Vector3 finalAcceleration = mAcceleration;
  finalAcceleration += mForces * mInverseMass;
  mVelocity += finalAcceleration * dt;
  mVelocity *= powf(mDamping, dt);

  GetOwner()->GET<Transform>()->SetPosition(position);

  // Nullify all forces
  mForces *= 0;

  // Update the size for broadphasing
  mBroadSize = GetOwner()->GET<Transform>()->GetSize() + Vector3(15.0f, 15.0f, 15.0f);
}

void PhysicsObject::SendMessage(Message const &aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

void PhysicsObject::ReceiveMessage(Message const &aMessage)
{

}

void PhysicsObject::AddForce(Vector3 const &aForce)
{
  mForces += aForce;
}

Vector3 PhysicsObject::GetVelocity() const
{
  return mVelocity;
}

Vector3 PhysicsObject::GetAcceleration() const
{
  return mAcceleration;
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
	mMass = aMass;
	mInverseMass = 1.0f / mMass;
}

bool PhysicsObject::IsStatic() const
{
  return mStatic;
}

void PhysicsObject::SetStatic(bool aStatic)
{
  mStatic = aStatic;
}

bool PhysicsObject::IsAffectedByGravity() const
{
  return mGravity;
}

void PhysicsObject::SetAffectedByGravity(bool aGravity)
{
  mGravity = aGravity;
}

Vector3 PhysicsObject::GetBroadSize() const
{
	return mBroadSize;
}
