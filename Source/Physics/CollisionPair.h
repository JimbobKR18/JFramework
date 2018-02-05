#ifndef __JFramework_CollisionPair_h_
#define __JFramework_CollisionPair_h_

#include "MathExt.h"
#include "Shape.h"

class PhysicsObject;

struct CollisionPair
{
  PhysicsObject*  mBodies[2];
  Shape*          mShapes[2];
  float           mPenetration;
  Vector3         mRelativeVelocity;
  Vector3         mNormal;
  Vector3         mContactPoint;

  CollisionPair(PhysicsObject* aBody1, PhysicsObject* aBody2, Shape* aShape1, Shape* aShape2);
  void operator=(CollisionPair const &rhs);
  bool operator==(CollisionPair const &rhs);
  void Switch();
};

#endif
