/*
 * CollisionChecker.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: jimbob
 */

#include "CollisionChecker.h"
#include "Transform.h"
#include "PhysicsObject.h"

bool CollisionChecker::CheckShapeCollision(CollisionPair &aPair)
{
  switch(aPair.mBodies[0]->mShape)
  {
  case PhysicsObject::SPHERE:
    switch(aPair.mBodies[1]->mShape)
    {
    case PhysicsObject::SPHERE:
      return CheckSphereToSphere(aPair);
    case PhysicsObject::CUBE:
      return CheckSphereToCube(aPair);
    }
    break;
  case PhysicsObject::CUBE:
    switch(aPair.mBodies[1]->mShape)
    {
    case PhysicsObject::SPHERE:
      return CheckSphereToCube(aPair);
    case PhysicsObject::CUBE:
      return CheckCubeToCube(aPair);
    }
    break;
  }
  // Default fallback
  return false;
}

bool CollisionChecker::CheckLineCollision(LineSegment const &aSegment, PhysicsObject *aBody)
{
  return false;
}

bool CollisionChecker::CheckSphereToSphere(CollisionPair &aPair)
{
  Transform *t1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *t2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  Vector3 t1Pos = t1->GetPosition();
  Vector3 t2Pos = t2->GetPosition();
  Vector3 t1Size = t1->GetSize();
  Vector3 t2Size = t2->GetSize();

  if(t1Size.x + t2Size.x > (t1Pos - t2Pos).length())
  {
    return true;
  }

  return false;
}

bool CollisionChecker::CheckSphereToCube(CollisionPair &aPair)
{
  if(aPair.mBodies[0]->mShape != PhysicsObject::SPHERE)
    aPair.Switch();

  Transform *t1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *t2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  Vector3 t1Pos = t1->GetPosition();
  Vector3 t2Pos = t2->GetPosition();
  Vector3 halfSize1 = t1->GetSize();
  Vector3 halfSize2 = t2->GetSize();

  for(int i = 0; i < 3; ++i)
  {
    float pos1 = t1Pos[i];
    float pos2 = t2Pos[i];

    if(fabs(pos1 - pos2) > (halfSize1[i] + halfSize2[i]))
      return false;
  }

  return true;
}

bool CollisionChecker::CheckCubeToCube(CollisionPair &aPair)
{
  Transform *t1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *t2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  Vector3 t1Pos = t1->GetPosition();
  Vector3 t2Pos = t2->GetPosition();
  Vector3 halfSize1 = t1->GetSize();
  Vector3 halfSize2 = t2->GetSize();

  bool xCheck = fabs(t1Pos.x - t2Pos.x) <= halfSize1.x + halfSize2.x;
  bool yCheck = fabs(t1Pos.y - t2Pos.y) <= halfSize1.y + halfSize2.y;
  bool zCheck = fabs(t1Pos.z - t2Pos.z) <= halfSize1.z + halfSize2.z;

  return xCheck && yCheck && zCheck;
}

