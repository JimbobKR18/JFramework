#include "Resolver.h"
#include "PhysicsObject.h"
#include "Transform.h"
#include "CollisionMessage.h"
#include "CollisionChecker.h"

Resolver::Resolver()
{
}

Resolver::~Resolver()
{
}

void Resolver::Update(float aDuration)
{
  for(std::list<PotentialPair>::iterator it = mPotentialPairs.begin(); it != mPotentialPairs.end(); ++it)
  {
    std::vector<CollisionPair> pairs = CollisionChecker::CheckShapeCollision(*it);
    for(std::vector<CollisionPair>::iterator it2 = pairs.begin(); it2 != pairs.end(); ++it2)
    {
      AddCollidedPair(*it2);
    }
    if(!pairs.empty())
    {
      SendCollisionMessages(*it);
    }
  }
  for(std::list<CollisionPair>::iterator it = mCollidedPairs.begin(); it != mCollidedPairs.end(); ++it)
  {
    Resolve(*it, aDuration);
  }
  mCollidedPairs.clear();
  mPotentialPairs.clear();
}

void Resolver::AddPrelimPair(PotentialPair const &aPair)
{
  mPotentialPairs.push_back(aPair);
}

void Resolver::AddCollidedPair(CollisionPair const &aPair)
{
  mCollidedPairs.push_back(aPair);
}

bool Resolver::Find(PhysicsObject *aObject1, PhysicsObject *aObject2)
{
  PotentialPair temp(aObject1, aObject2);

  for(std::list<PotentialPair>::iterator it = mPotentialPairs.begin(); it != mPotentialPairs.end(); ++it)
  {
    if(*it == temp)
    {
      return true;
    }
  }
  return false;
}

float Resolver::CalculateSeparatingVelocity(CollisionPair const &aPair)
{
  Vector3 separatingVelocity = aPair.mBodies[0]->GetVelocity();
  if(aPair.mBodies[1])
    separatingVelocity -= aPair.mBodies[1]->GetVelocity();
  return separatingVelocity * aPair.mNormal;
}

void Resolver::ResolvePenetration(CollisionPair const &aPair)
{
  if(aPair.mPenetration <= 0)
    return;

  float totalInverseMass = (1.0f / aPair.mBodies[0]->GetMass()) + 
                           (1.0f / aPair.mBodies[1]->GetMass());

  if(totalInverseMass <= 0)
    return;

  // Find the rate in which each object must move
  Vector3 movePerIMass = aPair.mNormal * (aPair.mPenetration / totalInverseMass);
  Vector3 b1Pos = aPair.mBodies[0]->GetOwner()->GET<Transform>()->GetPosition() +
                  aPair.mShapes[0]->position;
  Vector3 b2Pos = aPair.mBodies[1]->GetOwner()->GET<Transform>()->GetPosition() +
                  aPair.mShapes[1]->position;
  Vector3 b1Movement = (movePerIMass * (1.0f / aPair.mBodies[0]->GetMass())) /**
                        (aPair.mBodies[1]->IsStatic() ? 2.0f : 1.0f)*/;
  Vector3 b2Movement = (movePerIMass * (1.0f / aPair.mBodies[1]->GetMass())) /**
                          (aPair.mBodies[0]->IsStatic() ? 2.0f : 1.0f)*/;

  // Must check if objects can be moved
  if(!aPair.mBodies[0]->IsStatic())
  {
    aPair.mBodies[0]->GetOwner()->GET<Transform>()->SetPosition(b1Pos + b1Movement);
  }
  if(!aPair.mBodies[1]->IsStatic())
  {
    aPair.mBodies[1]->GetOwner()->GET<Transform>()->SetPosition(b2Pos - b2Movement);
  }
}

void Resolver::ResolveVelocity(CollisionPair const &aPair, float aDuration)
{
  float separatingVelocity = CalculateSeparatingVelocity(aPair);
  
  if(separatingVelocity > 0)
    return;
  
  float newSeparatingVelocity = -separatingVelocity * aPair.mRestitution;
  
  Vector3 accCausedVelocity = aPair.mBodies[0]->GetAcceleration();
  if(aPair.mBodies[1])
    accCausedVelocity -= aPair.mBodies[1]->GetAcceleration();
  
  float accCausedSepVelocity = accCausedVelocity * aPair.mNormal * aDuration;
  
  if(accCausedSepVelocity < 0)
  {
    newSeparatingVelocity += aPair.mRestitution * accCausedSepVelocity;
    if(newSeparatingVelocity < 0)
      newSeparatingVelocity = 0;
  }
  
  float deltaVelocity = newSeparatingVelocity - separatingVelocity;
  float totalInverseMass = 0.0f;
  if(aPair.mBodies[0])
    totalInverseMass += 1.0f / aPair.mBodies[0]->GetMass();
  if(aPair.mBodies[1])
    totalInverseMass += 1.0f / aPair.mBodies[1]->GetMass();
  
  if(totalInverseMass <= 0) return;
  
  float impulse = deltaVelocity / totalInverseMass;
  Vector3 impulsePerIMass = aPair.mNormal * impulse;
  
  Vector3 b1Movement = (impulsePerIMass * (1.0f / aPair.mBodies[0]->GetMass()));// *
                        //(aPair.mBodies[1]->IsStatic() ? 2.0f : 1.0f);
  Vector3 b2Movement = (impulsePerIMass * (1.0f / aPair.mBodies[1]->GetMass()));// *
                        //(aPair.mBodies[0]->IsStatic() ? 2.0f : 1.0f);
  
  if(aPair.mBodies[0] && !aPair.mBodies[0]->IsStatic())
    aPair.mBodies[0]->SetVelocity(aPair.mBodies[0]->GetVelocity() + b1Movement);
  
  if(aPair.mBodies[1] && !aPair.mBodies[1]->IsStatic())
    aPair.mBodies[1]->SetVelocity(aPair.mBodies[1]->GetVelocity() - b2Movement);
}

void Resolver::SendCollisionMessages(PotentialPair &aPair) const
{
  CollisionMessage message("", aPair.mBodies[0]->GetOwner(), aPair.mBodies[1]->GetOwner());
  aPair.mBodies[0]->GetOwner()->ReceiveMessage(message);
  aPair.mBodies[1]->GetOwner()->ReceiveMessage(message);
}

void Resolver::Resolve(CollisionPair &aPair, float aDuration)
{
  // Skip resolving if either object is passable.
  if(aPair.mBodies[0]->IsPassable() || aPair.mBodies[1]->IsPassable())
    return;
    
  switch(aPair.mShapes[0]->shape)
  {
  case Shape::SPHERE:
    switch(aPair.mShapes[1]->shape)
    {
    case Shape::SPHERE:
      CalculateSphereToSphere(aPair);
      break;
    case Shape::CUBE:
      CalculateSphereToCube(aPair);
      break;
    default:
      break;
    }
    break;
  case Shape::CUBE:
    switch(aPair.mShapes[1]->shape)
    {
    case Shape::SPHERE:
      CalculateSphereToCube(aPair);
      break;
    case Shape::CUBE:
      CalculateCubeToCube(aPair);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
  ResolveVelocity(aPair, aDuration);
  ResolvePenetration(aPair);
}

void Resolver::CalculateSphereToSphere(CollisionPair &aPair)
{
  Transform *b1Transform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *b2Transform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 b1Pos = b1Transform->GetPosition() + aPair.mShapes[0]->position;
  Vector3 b2Pos = b2Transform->GetPosition() + aPair.mShapes[1]->position;

  aPair.mPenetration = fabs((b1Pos - b2Pos).length() - (b1Transform->GetSize().x + b2Transform->GetSize().x));
  aPair.mNormal = (b1Pos - b2Pos).normalize();
  aPair.mRelativeVelocity = aPair.mBodies[1]->GetVelocity() - aPair.mBodies[0]->GetVelocity();
  aPair.mRestitution = 1.0f;
}

void Resolver::CalculateSphereToCube(CollisionPair &aPair)
{
  Transform *b1Transform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *b2Transform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 b1Pos = b1Transform->GetPosition() + aPair.mShapes[0]->position;
  Vector3 b2Pos = b2Transform->GetPosition() + aPair.mShapes[1]->position;
 
  int axis = 0;
  float shortestDistance = 0xffffff;
  
  for(int i = 0; i < 3; ++i)
  {
    float distance = fabs(fabs(b2Pos[i] - b1Pos[i]) - 
                      (aPair.mShapes[0]->GetSize(0) + aPair.mShapes[1]->GetSize(i)));
    if(distance < shortestDistance)
    {
      axis = i;
      shortestDistance = distance;
    }
  }
  
  Vector3 normal;
  // Figure out the normal
  switch(axis)
  {
    case 0:
      normal = Vector3(b1Pos[axis] - b2Pos[axis],0,0).normalize();
      break;
    case 1:
      normal = Vector3(0,b1Pos[axis] - b2Pos[axis],0).normalize();
      break;
    case 2:
      normal = Vector3(0,0,b1Pos[axis] - b2Pos[axis]).normalize();
      break;
  }

  aPair.mPenetration = fabs(fabs(b1Pos[axis] - b2Pos[axis]) - 
                        (aPair.mShapes[0]->GetSize(0) + aPair.mShapes[1]->GetSize(axis)));
  aPair.mNormal = normal;
  aPair.mRelativeVelocity = aPair.mBodies[1]->GetVelocity() - aPair.mBodies[0]->GetVelocity();
  aPair.mRestitution = 1.0f;
}

void Resolver::CalculateCubeToCube(CollisionPair &aPair)
{
  Transform *b1Transform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *b2Transform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 b1Pos = b1Transform->GetPosition() + aPair.mShapes[0]->position;
  Vector3 b2Pos = b2Transform->GetPosition() + aPair.mShapes[1]->position;
  
  int axis = 0;
  float shortestDistance = 0xffffff;
  
  for(int i = 0; i < 3; ++i)
  {
    float distance = fabs(fabs(b2Pos[i] - b1Pos[i]) - 
                      (aPair.mShapes[0]->GetSize(i) + aPair.mShapes[1]->GetSize(i)));
    if(distance < shortestDistance)
    {
      axis = i;
      shortestDistance = distance;
    }
  }
  
  Vector3 normal;
  // Figure out the normal
  switch(axis)
  {
    case 0:
      normal = Vector3(b1Pos[axis] - b2Pos[axis],0,0).normalize();
      break;
    case 1:
      normal = Vector3(0,b1Pos[axis] - b2Pos[axis],0).normalize();
      break;
    case 2:
      normal = Vector3(0,0,b1Pos[axis] - b2Pos[axis]).normalize();
      break;
  }
  
  aPair.mPenetration = fabs(fabs(b1Pos[axis] - b2Pos[axis]) - 
                        (aPair.mShapes[0]->GetSize(axis) + aPair.mShapes[1]->GetSize(axis)));
  aPair.mNormal = normal;
  aPair.mRelativeVelocity = aPair.mBodies[1]->GetVelocity() - aPair.mBodies[0]->GetVelocity();
  aPair.mRestitution = 1.0f;
}
