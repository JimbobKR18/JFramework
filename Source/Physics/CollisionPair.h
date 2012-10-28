#ifndef __JFramework_CollisionPair_h_
#define __JFramework_CollisionPair_h_

#include "PhysicsObject.h"

struct CollisionPair
{
	PhysicsObject *mBodies[2];

	CollisionPair(PhysicsObject *aBody1, PhysicsObject *aBody2);
	void operator=(CollisionPair const &rhs);
	void Switch();
};

#endif
