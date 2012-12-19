#include "Resolver.h"
#include "PhysicsObject.h"
#include "Transform.h"

Resolver::Resolver()
{

}

Resolver::~Resolver()
{

}

void Resolver::Update()
{
  for(std::list<CollisionPair>::iterator it = mBroadPairs.begin(); it != mBroadPairs.end(); ++it)
  {
    CheckCollision(*it);
  }
	for(std::list<CollisionPair>::iterator it = mPairs.begin(); it != mPairs.end(); ++it)
	{
		Resolve(*it);
	}
	mPairs.clear();
	mBroadPairs.clear();
}

void Resolver::AddPrelimPair(CollisionPair const &aPair)
{
  mBroadPairs.push_back(aPair);
}

void Resolver::AddPair(CollisionPair const &aPair)
{
	mPairs.push_back(aPair);
}

bool Resolver::Find(PhysicsObject *aObject1, PhysicsObject *aObject2)
{
	CollisionPair temp(aObject1, aObject2);

	for(std::list<CollisionPair>::iterator it = mPairs.begin(); it != mPairs.end(); ++it)
	{
		if(*it == temp)
		{
			return true;
		}
	}
	for(std::list<CollisionPair>::iterator it = mBroadPairs.begin(); it != mBroadPairs.end(); ++it)
  {
    if(*it == temp)
    {
      return true;
    }
  }
	return false;
}

void Resolver::ResolvePenetration(CollisionPair const &aPair)
{
  if(aPair.mPenetration <= 0)
    return;

  float totalInverseMass = (1.0f / aPair.mBodies[0]->GetMass()) + (1.0f / aPair.mBodies[1]->GetMass());

  if(totalInverseMass <= 0)
    return;

  // Find the rate in which each object must move
  Vector3 movePerIMass = aPair.mNormal * (-aPair.mPenetration / totalInverseMass);
  Vector3 b1Pos = aPair.mBodies[0]->GetOwner()->GET<Transform>()->GetPosition();
  Vector3 b2Pos = aPair.mBodies[1]->GetOwner()->GET<Transform>()->GetPosition();
  Vector3 b1Movement = (movePerIMass * (1.0f / aPair.mBodies[0]->GetMass())) *
                        (aPair.mBodies[1]->IsStatic() ? 2.0f : 1.0f);
  Vector3 b2Movement = (movePerIMass * (1.0f / aPair.mBodies[1]->GetMass())) *
                          (aPair.mBodies[0]->IsStatic() ? 2.0f : 1.0f);

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

void Resolver::ResolveVelocity(CollisionPair const &aPair)
{
  aPair.mBodies[0]->SetVelocity(Vector3(0,0,0));
  aPair.mBodies[1]->SetVelocity(Vector3(0,0,0));
}

void Resolver::Resolve(CollisionPair &aPair)
{
	switch(aPair.mBodies[0]->mShape)
	{
	case PhysicsObject::SPHERE:
		switch(aPair.mBodies[1]->mShape)
		{
		case PhysicsObject::SPHERE:
			CalculateSphereToSphere(aPair);
			break;
		case PhysicsObject::CUBE:
		  CalculateSphereToCube(aPair);
			break;
		}
		break;
	case PhysicsObject::CUBE:
		switch(aPair.mBodies[1]->mShape)
		{
		case PhysicsObject::SPHERE:
		  CalculateSphereToCube(aPair);
			break;
		case PhysicsObject::CUBE:
		  CalculateCubeToCube(aPair);
			break;
		}
		break;
	}
	ResolvePenetration(aPair);
	ResolveVelocity(aPair);
}

void Resolver::CheckCollision(CollisionPair &aPair)
{
  switch(aPair.mBodies[0]->mShape)
  {
  case PhysicsObject::SPHERE:
    switch(aPair.mBodies[1]->mShape)
    {
    case PhysicsObject::SPHERE:
      if(CheckSphereToSphere(aPair))
        AddPair(aPair);
      break;
    case PhysicsObject::CUBE:
      if(CheckSphereToCube(aPair))
        AddPair(aPair);
      break;
    }
    break;
  case PhysicsObject::CUBE:
    switch(aPair.mBodies[1]->mShape)
    {
    case PhysicsObject::SPHERE:
      if(CheckSphereToCube(aPair))
        AddPair(aPair);
      break;
    case PhysicsObject::CUBE:
      if(CheckCubeToCube(aPair))
        AddPair(aPair);
      break;
    }
    break;
  }
}

bool Resolver::CheckSphereToSphere(CollisionPair &aPair)
{
  Transform *t1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *t2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  if(t1->GetSize().x + t2->GetSize().x > (t1->GetPosition() - t2->GetPosition()).length())
  {
    return true;
  }

  return false;
}

bool Resolver::CheckSphereToCube(CollisionPair &aPair)
{
  if(aPair.mBodies[0]->mShape != PhysicsObject::SPHERE)
    aPair.Switch();

  Transform *t1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *t2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  Vector3 halfSize1 = t1->GetSize()/2.0f;
  Vector3 halfSize2 = t2->GetSize()/2.0f;

  for(int i = 0; i < 3; ++i)
  {
    float pos1 = t1->GetPosition()[i];
    float pos2 = t2->GetPosition()[i];

    if(fabs(pos1 - pos2) > (halfSize1[0] + halfSize2[i]))
      return false;
  }

  return true;
}

bool Resolver::CheckCubeToCube(CollisionPair &aPair)
{
  Transform *t1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *t2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  Vector3 halfSize1 = t1->GetSize()/2.0f;
   Vector3 halfSize2 = t2->GetSize()/2.0f;

  bool xCheck = fabs(t1->GetPosition().x - t2->GetPosition().x) < halfSize1.x + halfSize2.x;
  bool yCheck = fabs(t1->GetPosition().y - t2->GetPosition().y) < halfSize1.y + halfSize2.z;
  bool zCheck = fabs(t1->GetPosition().z - t2->GetPosition().z) < halfSize1.z + halfSize2.y;

  return xCheck && yCheck && zCheck;
}

void Resolver::CalculateSphereToSphere(CollisionPair &aPair)
{
  Transform *b1Transform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *b2Transform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 b1Pos = b1Transform->GetPosition();
  Vector3 b2Pos = b2Transform->GetPosition();

  aPair.mPenetration = fabs((b1Pos - b2Pos).length() - (b1Transform->GetSize().x + b2Transform->GetSize().x));
  aPair.mNormal = (b2Pos - b1Pos).normalize();
  aPair.mRelativeVelocity = aPair.mBodies[1]->GetVelocity() - aPair.mBodies[0]->GetVelocity();
}

void Resolver::CalculateSphereToCube(CollisionPair &aPair)
{
  Transform *b1Transform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *b2Transform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 b1Pos = b1Transform->GetPosition();
  Vector3 b2Pos = b2Transform->GetPosition();

  aPair.mPenetration = fabs(((b2Pos - b1Pos).normalize() * (b1Pos - b2Pos).length()).length());
  aPair.mNormal = (b2Pos - b1Pos).normalize();
  aPair.mRelativeVelocity = aPair.mBodies[1]->GetVelocity() - aPair.mBodies[0]->GetVelocity();
}

void Resolver::CalculateCubeToCube(CollisionPair &aPair)
{

}
