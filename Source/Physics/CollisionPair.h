#ifndef __JFramework_CollisionPair_h_
#define __JFramework_CollisionPair_h_

#include "MathExt.h"

class PhysicsObject;

struct CollisionPair
{
	PhysicsObject*  mBodies[2];
	float           mPenetration,
                  mRestitution;
	Vector3         mRelativeVelocity,
                  mNormal;

	CollisionPair(PhysicsObject *aBody1, PhysicsObject *aBody2);
	void operator=(CollisionPair const &rhs);
	bool operator==(CollisionPair const &rhs);
	void Switch();
};

#endif
