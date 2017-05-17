#include "PotentialPair.h"
#include "PhysicsObject.h"

PotentialPair::PotentialPair()
{
  mBodies[0] = nullptr;
  mBodies[1] = nullptr;
  mDistance = 0;
}

PotentialPair::PotentialPair(PhysicsObject* mBody1, PhysicsObject* mBody2, float aDistance)
{
  mBodies[0] = mBody1;
  mBodies[1] = mBody2;
  mDistance = aDistance;
}

void PotentialPair::operator=(PotentialPair const &rhs)
{
  mBodies[0] = rhs.mBodies[0];
  mBodies[1] = rhs.mBodies[1];
  mDistance = rhs.mDistance;
}

bool PotentialPair::operator==(PotentialPair const &rhs) const
{
  if((mBodies[0] == rhs.mBodies[0] && mBodies[1] == rhs.mBodies[1]) ||
      (mBodies[0] == rhs.mBodies[1] && mBodies[1] == rhs.mBodies[0]))
  {
    return true;
  }
  return false;
}

bool PotentialPair::operator<(PotentialPair const &rhs) const
{
  return mDistance < rhs.mDistance;
}
