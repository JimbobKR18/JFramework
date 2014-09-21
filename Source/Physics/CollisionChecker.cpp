/*
 * CollisionChecker.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: jimbob
 */

#include "CollisionChecker.h"
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

bool CollisionChecker::CheckLineCollision(Line const &aSegment, PhysicsObject *aBody)
{
  switch(aBody->mShape)
  {
  case PhysicsObject::SPHERE:
    return CheckLineToSphere(aSegment, aBody->GetOwner()->GET<Transform>());
  case PhysicsObject::CUBE:
    return CheckLineToCube(aSegment, aBody->GetOwner()->GET<Transform>());
  default:
    return false;
  }
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

bool CollisionChecker::CheckLineToSphere(Line const &aSegment, Transform *aSphere)
{
  // http://bit.ly/1uFUfXK
  Vector3 end = aSegment.position + (aSegment.direction * aSegment.length);
  Vector3 position = aSphere->GetPosition();
  float radius = aSphere->GetSize().x;
  Vector3 direction = (end - aSegment.position).normalize();
  Vector3 oc = aSegment.position - position;
  float l = (direction.Dot(oc));
  float d = (l * l) - (oc.length() * oc.length()) + (radius * radius);
  return d >= 0.0f;
}

bool CollisionChecker::CheckLineToCube(Line const &aSegment, Transform *aCube)
{
  // http://bit.ly/1p1SX3G
  const int RIGHT = 0;
  const int LEFT = 1;
  const int CENTER = 2;
  bool inside = true;
  int quadrant[3];
  int whichPlane;
  float maxT[3];
  float candidatePlane[3];
  Vector3 position = aSegment.position;
  Vector3 direction = aSegment.direction;
  Vector3 min = aCube->GetPosition() - aCube->GetSize();
  Vector3 max = aCube->GetPosition() + aCube->GetSize();
  Vector3 collision;

  /* Find candidate planes; this loop can be avoided if
      rays cast all from the eye(assume perspective view) */
  for(int i = 0; i < 3; ++i)
  {
    if(position[i] < min[i])
    {
      quadrant[i] = LEFT;
      candidatePlane[i] = min[i];
      inside = false;
    }
    else if(position[i] > max[i])
    {
      quadrant[i] = RIGHT;
      candidatePlane[i] = max[i];
      inside = false;
    }
    else
    {
      quadrant[i] = CENTER;
    }
  }

  // Line origin inside cube
  if(inside)
  {
    collision = aSegment.position;
    return true;
  }

  /* Calculate T distances to candidate planes */
  for(int i = 0; i < 3; ++i)
  {
    if(quadrant[i] != CENTER && direction[i] != 0)
    {
      maxT[i] = (candidatePlane[i] - position[i]) / direction[i];
    }
    else
    {
      maxT[i] = -1.0f;
    }
  }

  /* Get largest of the maxT's for final choice of intersection */
  whichPlane = 0;
  for(int i = 1; i < 3; ++i)
  {
    if(maxT[whichPlane] < maxT[i])
    {
      whichPlane = i;
    }
  }

  /* Check final candidate actually inside box */
  if(maxT[whichPlane] < 0) return false;
  for(int i = 0; i < 3; ++i)
  {
    if(whichPlane != i)
    {
      collision[i] = position[i] + maxT[whichPlane] * direction[i];
      if(collision[i] < min[i] || collision[i] > max[i])
      {
        return false;
      }
    }
    else
    {
        collision[i] = candidatePlane[i];
    }
  }

  return true;
}

