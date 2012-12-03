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
	for(std::list<CollisionPair>::iterator it = mPairs.begin(); it != mPairs.end(); ++it)
	{
		Resolve(*it);
	}
	mPairs.clear();
}

void Resolver::AddPair(CollisionPair const &aPair)
{
	mPairs.push_back(aPair);
}

bool Resolver::Find(PhysicsObject *aObject1, PhysicsObject *aObject2)
{
	CollisionPair temp(aObject1, aObject2);

	for(std::list<CollisionPair>::iterator it = mPairs.begin();
		it != mPairs.end(); ++it)
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

  Vector3 movePerIMass = aPair.mNormal * (-aPair.mPenetration / totalInverseMass);
  Vector3 b1Pos = aPair.mBodies[0]->GetOwner()->GET<Transform>()->GetPosition();
  Vector3 b2Pos = aPair.mBodies[1]->GetOwner()->GET<Transform>()->GetPosition();


  aPair.mBodies[0]->GetOwner()->GET<Transform>()->SetPosition(b1Pos + (movePerIMass * (1.0f / aPair.mBodies[0]->GetMass())));
  aPair.mBodies[1]->GetOwner()->GET<Transform>()->SetPosition(b2Pos + (movePerIMass * (1.0f / aPair.mBodies[1]->GetMass())));
}

void Resolver::ResolveVelocity(CollisionPair const &aPair)
{

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

}

void Resolver::CalculateCubeToCube(CollisionPair &aPair)
{

}
