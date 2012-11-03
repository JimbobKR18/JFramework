#include "CollisionPair.h"
#include "PhysicsObject.h"

CollisionPair::CollisionPair(PhysicsObject *aBody1, PhysicsObject *aBody2)
{
	mBodies[0] = aBody1;
	mBodies[1] = aBody2;
}

void CollisionPair::operator=(CollisionPair const &rhs)
{
	mBodies[0] = rhs.mBodies[0];
	mBodies[1] = rhs.mBodies[1];
}

bool CollisionPair::operator==(CollisionPair const &rhs)
{
	if((mBodies[0] == rhs.mBodies[0] && mBodies[1] == rhs.mBodies[1]) ||
	   (mBodies[0] == rhs.mBodies[1] && mBodies[1] == rhs.mBodies[0]))
	{
		return true;
	}
	return false;
}

void CollisionPair::Switch()
{
	PhysicsObject *temp = mBodies[0];
	mBodies[0] = mBodies[1];
	mBodies[1] = temp;
}
