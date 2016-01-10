#include "CollisionPair.h"
#include "PhysicsObject.h"

CollisionPair::CollisionPair(PhysicsObject* aBody1, PhysicsObject* aBody2, Shape* aShape1, Shape* aShape2) : 
  mPenetration(0), mRestitution(0), mRelativeVelocity(), mNormal()
{
  mBodies[0] = aBody1;
  mBodies[1] = aBody2;
  mShapes[0] = aShape1;
  mShapes[1] = aShape2;
}

void CollisionPair::operator=(CollisionPair const &rhs)
{
  mShapes[0] = rhs.mShapes[0];
  mShapes[1] = rhs.mShapes[1];
  mBodies[0] = rhs.mBodies[0];
  mBodies[1] = rhs.mBodies[1];
  mPenetration = rhs.mPenetration;
  mRestitution = rhs.mRestitution;
  mRelativeVelocity = rhs.mRelativeVelocity;
  mNormal = rhs.mNormal;
}

bool CollisionPair::operator==(CollisionPair const &rhs)
{
  if(((mShapes[0] == rhs.mShapes[0] && mShapes[1] == rhs.mShapes[1]) ||
      (mShapes[0] == rhs.mShapes[1] && mShapes[1] == rhs.mShapes[0])) &&
      ((mBodies[0] == rhs.mBodies[0] && mBodies[1] == rhs.mBodies[1]) ||
      (mBodies[0] == rhs.mBodies[1] && mBodies[1] == rhs.mBodies[0])))
  {
    return true;
  }
  return false;
}

/**
 * @brief Switch sides for objects.
 */
void CollisionPair::Switch()
{
  Shape* temp = mShapes[0];
  mShapes[0] = mShapes[1];
  mShapes[1] = temp;
  
  PhysicsObject* temp2 = mBodies[0];
  mBodies[0] = mBodies[1];
  mBodies[1] = temp2;
}
