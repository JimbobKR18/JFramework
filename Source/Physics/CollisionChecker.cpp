/*
 * CollisionChecker.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: jimbob
 */

#include "CollisionChecker.h"
#include "PhysicsObject.h"

std::vector<CollisionPair> CollisionChecker::CheckShapeCollision(PotentialPair const &aPair)
{
  std::vector<CollisionPair> pairs;
  PhysicsObject::constShapeIT it1End = aPair.mBodies[0]->GetShapes().end();
  PhysicsObject::constShapeIT it2End = aPair.mBodies[1]->GetShapes().end();
  for(PhysicsObject::constShapeIT it = aPair.mBodies[0]->GetShapes().begin();
      it != it1End; ++it)
  {
    for(PhysicsObject::constShapeIT it2 = aPair.mBodies[1]->GetShapes().begin();
      it2 != it2End; ++it2)
    {
      bool collided = false;
      CollisionPair potentialPair(aPair.mBodies[0], aPair.mBodies[1], *it, *it2);
        
      switch((*it)->shape)
      {
      case Shape::SPHERE:
        switch((*it2)->shape)
        {
        case Shape::SPHERE:
          collided = CheckSphereToSphere(potentialPair);
          break;
        case Shape::CUBE:
          collided = CheckSphereToCube(potentialPair);
          break;
        default:
          break;
        }
        break;
      case Shape::CUBE:
        switch((*it2)->shape)
        {
        case Shape::SPHERE:
          collided = CheckSphereToCube(potentialPair);
          break;
        case Shape::CUBE:
          collided = CheckCubeToCube(potentialPair);
          break;
        default:
          break;
        }
        break;
      default:
        break;
      }
      
      if(collided)
        pairs.push_back(potentialPair);
    }
  }
  return pairs;
}

bool CollisionChecker::CheckLineCollision(Line const &aSegment, Transform* aTransform, Shape* aShape)
{
  switch(aShape->shape)
  {
  case Shape::SPHERE:
    return CheckLineToSphere(aSegment, aTransform, aShape);
    break;
  case Shape::CUBE:
    return CheckLineToCube(aSegment, aTransform, aShape);
    break;
  default:
    return false;
    break;
  }
}

bool CollisionChecker::CheckSphereToSphere(CollisionPair &aPair)
{
  Transform *t1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *t2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  Vector3 t1Pos = t1->GetPosition() + aPair.mShapes[0]->position;
  Vector3 t2Pos = t2->GetPosition() + aPair.mShapes[1]->position;
  float t1Size = aPair.mShapes[0]->GetSize(0);
  float t2Size = aPair.mShapes[1]->GetSize(0);

  if(t1Size + t2Size > (t1Pos - t2Pos).length())
  {
    return true;
  }

  return false;
}

bool CollisionChecker::CheckSphereToCube(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape != Shape::SPHERE)
    aPair.Switch();

  Transform *sphere = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *cube = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  Vector3 spherePos = sphere->GetPosition() + aPair.mShapes[0]->position;
  Vector3 cubePos = cube->GetPosition() + aPair.mShapes[1]->position;
  Vector3 relPos = spherePos - cubePos;
  
  Vector3 closestPoint;

  for(int i = 0; i < 3; ++i)
  {
    float cubeSize = aPair.mShapes[1]->GetSize(i);
    
    if(relPos[i] < -cubeSize)
    {
      closestPoint[i] = -cubeSize;
    }
    else if(relPos[i] > cubeSize)
    {
      closestPoint[i] = cubeSize;
    }
    else
    {
      closestPoint[i] = relPos[i];
    }
  }

  Vector3 dist = relPos - closestPoint;
  return dist.x*dist.x + dist.y*dist.y + dist.z*dist.z < aPair.mShapes[0]->GetSize(0)*aPair.mShapes[0]->GetSize(0);
}

bool CollisionChecker::CheckCubeToCube(CollisionPair &aPair)
{
  Transform *t1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *t2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  Vector3 t1Pos = t1->GetPosition() + aPair.mShapes[0]->position;
  Vector3 t2Pos = t2->GetPosition() + aPair.mShapes[1]->position;

  bool xCheck = fabs(t1Pos.x - t2Pos.x) <= aPair.mShapes[0]->GetSize(0) + aPair.mShapes[1]->GetSize(0);
  bool yCheck = fabs(t1Pos.y - t2Pos.y) <= aPair.mShapes[0]->GetSize(1) + aPair.mShapes[1]->GetSize(1);
  bool zCheck = fabs(t1Pos.z - t2Pos.z) <= aPair.mShapes[0]->GetSize(2) + aPair.mShapes[1]->GetSize(2);

  return xCheck && yCheck && zCheck;
}

bool CollisionChecker::CheckLineToSphere(Line const &aSegment, Transform *aSphere, Shape* aShape)
{
  // http://bit.ly/1uFUfXK
  Vector3 end = aSegment.position + (aSegment.direction * aSegment.length);
  Vector3 position = aSphere->GetPosition() + aShape->position;
  float radius = aShape->GetSize(0);
  Vector3 direction = (end - aSegment.position).normalize();
  Vector3 oc = aSegment.position - position;
  float l = (direction.Dot(oc));
  float d = (l * l) - (oc.length() * oc.length()) + (radius * radius);
  return d >= 0.0f;
}

bool CollisionChecker::CheckLineToCube(Line const &aSegment, Transform *aCube, Shape* aShape)
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
  Vector3 min = Vector3(aCube->GetPosition().x + aShape->position.x - aShape->GetSize(0),
                        aCube->GetPosition().y + aShape->position.y - aShape->GetSize(1),
                        aCube->GetPosition().z + aShape->position.z - aShape->GetSize(2));
  Vector3 max = Vector3(aCube->GetPosition().x + aShape->position.x + aShape->GetSize(0),
                        aCube->GetPosition().y + aShape->position.y + aShape->GetSize(1),
                        aCube->GetPosition().z + aShape->position.z + aShape->GetSize(2));
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

