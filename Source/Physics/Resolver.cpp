#include "Resolver.h"

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
}

void Resolver::AddPair(CollisionPair const &aPair)
{
	mPairs.push_back(aPair);
}

void Resolver::Resolve(CollisionPair const &aPair)
{
	switch(aPair.mBodies[0]->mShape)
	{
	case PhysicsObject::SPHERE:
		switch(aPair.mBodies[1]->mShape)
		{
		case PhysicsObject::SPHERE:
			SolveSphereToSphere(aPair);
			break;
		case PhysicsObject::CUBE:
			SolveSphereToCube(aPair);
			break;
		}
		break;
	case PhysicsObject::CUBE:
		switch(aPair.mBodies[1]->mShape)
		{
		case PhysicsObject::SPHERE:
			SolveSphereToCube(aPair);
			break;
		case PhysicsObject::CUBE:
			SolveCubeToCube(aPair);
			break;
		}
		break;
	}
}

void Resolver::SolveSphereToSphere(CollisionPair const &aPair)
{

}

void Resolver::SolveSphereToCube(CollisionPair const &aPair)
{

}

void Resolver::SolveCubeToCube(CollisionPair const &aPair)
{

}
