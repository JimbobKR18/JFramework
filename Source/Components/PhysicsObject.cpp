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
#include "ObjectManager.h"
#include "InputManager.h"
#include "LUATypes.h"
#include "CollisionMessage.h"
#include "PhysicsWorld.h"
#include "ObjectManager.h"

PhysicsObject::PhysicsObject(PhysicsWorld *aWorld) : Component("PhysicsObject"), mWorld(aWorld),
						     mVelocity(0,0,0), mAcceleration(0,0,0), mForces(0,0,0),
						     mBroadSize(0,0,0), mMass(0), mInverseMass(0), 
						     mDamping(0.01f), mStatic(false), mGravity(true), mShape(SPHERE)
{
}

PhysicsObject::~PhysicsObject()
{
  mWorld->GetOwningApp()->GET<PhysicsWorld>()->RemoveObject(this);
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
  mBroadSize = GetOwner()->GET<Transform>()->GetSize() * 1.5f;
}

void PhysicsObject::SendMessage(Message const &aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

void PhysicsObject::ReceiveMessage(Message const &aMessage)
{
  if(aMessage.GetDescription() != "Collision")
    return;

  CollisionMessage *message = (CollisionMessage*)&aMessage;
  GameObject *otherBody = message->GetObject(0) != GetOwner() ?
                          message->GetObject(0) : message->GetObject(1);
  std::string objectName = GetOwner()->GetFilename().substr(0, GetOwner()->GetFilename().length() - 4);
  
  if(!LUABind::LoadFunction<GameObject*>("CollisionMessages.LUA",
                                     objectName + "_CollisionReceive",
                                     otherBody))
  {
    // Do some fallback logic here
    otherBody->Interact(GetOwner());
  }
}

void PhysicsObject::Serialize(Parser &aParser)
{
  std::string objectName = std::string("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  Root* object = aParser.Find(objectName);
  std::string shape = mShape == PhysicsObject::CUBE ? "CUBE" : "SPHERE";

  object->Place(objectName, "PhysicsObject", "");
  object->Place("PhysicsObject", "Shape", shape);
  object->Place("PhysicsObject", "Gravity", Common::BoolToString(IsAffectedByGravity()));
  object->Place("PhysicsObject", "Static", Common::BoolToString(mStatic));
  object->Place("PhysicsObject", "Mass", Common::FloatToString(mMass));
  object->Place("PhysicsObject", "Damping", Common::FloatToString(mDamping));
}

void PhysicsObject::Deserialize(Parser &aParser)
{
  // What shape is our object? Is it affected by gravity?
  // What is the object's mass? Is it static?
  HashString type = aParser.Find("PhysicsObject", "Shape")->GetValue();
  bool gravity = aParser.Find("PhysicsObject", "Gravity")->GetValue().ToBool();
  bool isStatic = aParser.Find("PhysicsObject", "Static")->GetValue().ToBool();
  SetMass(aParser.Find("PhysicsObject", "Mass")->GetValue().ToInt());
  mDamping = aParser.Find("PhysicsObject", "Damping")->GetValue().ToFloat();

  //TODO serialize ignore list for collisions

  // default true
  if(!gravity)
  {
    GetOwner()->GetManager()->GetOwningApp()->GET<PhysicsWorld>()->UnregisterGravity(this);
    SetAffectedByGravity(false);
  }
  
  SetStatic(isStatic);
  
  if(type == "CUBE")
    mShape = PhysicsObject::CUBE;
  else if(type == "SPHERE")
    mShape = PhysicsObject::SPHERE;
  else
    assert(!"Invalid shape given");
}

void PhysicsObject::AddForce(Vector3 const &aForce)
{
  mForces += aForce;
}

void PhysicsObject::AddIgnore(std::string const &aObjectName)
{
  mIgnoreList.push_back(aObjectName);
}

bool PhysicsObject::IgnoreObject(std::string const &aObjectName)
{
  std::vector<std::string>::iterator end = mIgnoreList.end();
  for(std::vector<std::string>::iterator it = mIgnoreList.begin(); it != end; ++it)
  {
    if(aObjectName == *it)
      return true;
  }
  return false;
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

void PhysicsObject::SetBroadSize(Vector3 const &aSize)
{
  mBroadSize = aSize;
}
