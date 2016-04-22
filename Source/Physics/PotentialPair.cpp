#include "PotentialPair.h"
#include "PhysicsObject.h"

PotentialPair::PotentialPair()
{
  mBodies[0] = nullptr;
  mBodies[1] = nullptr;
}

PotentialPair::PotentialPair(PhysicsObject* mBody1, PhysicsObject* mBody2)
{
  mBodies[0] = mBody1;
  mBodies[1] = mBody2;
}

void PotentialPair::operator=(PotentialPair const &rhs)
{
  mBodies[0] = rhs.mBodies[0];
  mBodies[1] = rhs.mBodies[1];
}

bool PotentialPair::operator==(PotentialPair const &rhs)
{
  if((mBodies[0] == rhs.mBodies[0] && mBodies[1] == rhs.mBodies[1]) ||
      (mBodies[0] == rhs.mBodies[1] && mBodies[1] == rhs.mBodies[0]))
  {
    return true;
  }
  return false;
}