/*
 * CollisionChecker.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: jimbob
 */

#include "CollisionChecker.h"
#include "PhysicsObject.h"

/**
 * @brief General catch all shape collision caller.
 * @param aPair
 */
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
        case Shape::TRIANGLE:
          collided = CheckTriangleToSphere(potentialPair);
          break;
        case Shape::LINE:
          collided = CheckLineToSphere(potentialPair);
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
        case Shape::TRIANGLE:
          collided = CheckTriangleToCube(potentialPair);
          break;
        case Shape::LINE:
          collided = CheckLineToCube(potentialPair);
          break;
        default:
          break;
        }
        break;
      case Shape::TRIANGLE:
        switch((*it2)->shape)
        {
        case Shape::SPHERE:
          collided = CheckTriangleToSphere(potentialPair);
          break;
        case Shape::CUBE:
          collided = CheckTriangleToCube(potentialPair);
          break;
        case Shape::TRIANGLE:
          collided = CheckTriangleToTriangle(potentialPair);
          break;
        case Shape::LINE:
          collided = CheckLineToTriangle(potentialPair);
          break;
        default:
          break;
        }
        break;
      case Shape::LINE:
        switch((*it2)->shape)
        {
        case Shape::SPHERE:
          collided = CheckLineToSphere(potentialPair);
          break;
        case Shape::CUBE:
          collided = CheckLineToCube(potentialPair);
          break;
        case Shape::TRIANGLE:
          collided = CheckLineToTriangle(potentialPair);
          break;
        case Shape::LINE:
          collided = CheckLineToLine(potentialPair);
          break;
        default:
          break;
        }
      default:
        break;
      }
      
      if(collided)
        pairs.push_back(potentialPair);
    }
  }
  return pairs;
}

/**
 * @brief General catch all line collision caller.
 * @param aSegment Line
 * @param aTransform Object Transform
 * @param aShape Shape to check collision with
 */
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

/**
 * @brief Sphere to sphere check
 * @param aPair
 */
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

/**
 * @brief Sphere to cube check
 * @param aPair
 */
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

/**
 * @brief Cube to cube check
 * @param aPair
 */
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

/**
 * @brief Triangle to sphere check
 * @param aPair
 */
/* http://realtimecollisiondetection.net/blog/?p=103 */
bool CollisionChecker::CheckTriangleToSphere(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::SPHERE)
    aPair.Switch();
    
  Triangle* triangle = (Triangle*)aPair.mShapes[0];
  Sphere* sphere = (Sphere*)aPair.mShapes[1];
  Transform* triTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* sphereTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 spherePos = sphere->position + sphereTransform->GetPosition();
  for(int i = 0; i < 3; ++i)
  {
    // Trivial intersection of vertices test
    Vector3 vecPos = triTransform->GetPosition() + triangle->points[i];
    Vector3 dist = vecPos - spherePos;
    if(dist.length() < sphere->GetSize(0))
    {
      return true;
    }
    
    // Line segment test
    int j = i + 1;
    if (j > 2)
      j = 0;
    
    Line line(vecPos, triTransform->GetPosition() + triangle->points[j]);
    if(CheckLineToSphere(line, sphereTransform, sphere))
    {
      return true;
    }
    
    // TODO Check if sphere inside of triangle
  }
  return false;
}

/**
 * @brief Triangle to Cube check
 * @param aPair
 */
bool CollisionChecker::CheckTriangleToCube(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::CUBE)
    aPair.Switch();
    
  Triangle* triangle = (Triangle*)aPair.mShapes[0];
  Cube* cube = (Cube*)aPair.mShapes[1];
  Transform* triTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* cubeTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 cubePos = cube->position + cubeTransform->GetPosition();
  
  for(int i = 0; i < 3; ++i)
  {
    // Trivial intersection of vertices test
    Vector3 vecPos = triTransform->GetPosition() + triangle->points[i];
    Vector3 dist = vecPos - cubePos;
    
    // Separating axis test
    bool xCheck = fabs(dist.x) <= aPair.mShapes[1]->GetSize(0);
    bool yCheck = fabs(dist.y) <= aPair.mShapes[1]->GetSize(1);
    bool zCheck = fabs(dist.z) <= aPair.mShapes[1]->GetSize(2);

    if(xCheck && yCheck && zCheck)
      return true;
    
    // Line segment test
    int j = i + 1;
    if (j > 2)
      j = 0;
    
    Line line(vecPos, triTransform->GetPosition() + triangle->points[j]);
    if(CheckLineToCube(line, cubeTransform, cube))
    {
      return true;
    }
    
    // TODO Check if cube inside of triangle
  }
    
  return false;
}

/**
 * @brief Triangle to Triangle check.
 * @param aPair
 */
bool CollisionChecker::CheckTriangleToTriangle(CollisionPair &aPair)
{
  // TODO
  return false;
}

/**
 * @brief Line to Sphere check
 * @param aPair
 * @return true if collided
 */
bool CollisionChecker::CheckLineToSphere(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::SPHERE)
    aPair.Switch();
  
  // Move line into world space
  // This is a bit slow since it makes a new line every time.
  Transform *lineTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Line* line = (Line*)(aPair.mShapes[0]);
  Line tempLine = (*line);
  tempLine.position += lineTransform->GetPosition();
    
  return CheckLineToSphere(tempLine, aPair.mBodies[1]->GetOwner()->GET<Transform>(), aPair.mShapes[1]);
}

/**
 * @brief Line to Cube check
 * @param aPair
 * @return true if collided
 */
bool CollisionChecker::CheckLineToCube(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::CUBE)
    aPair.Switch();
    
  // Move line into world space
  // This is a bit slow since it makes a new line every time.
  Transform *lineTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Line* line = (Line*)(aPair.mShapes[0]);
  Line tempLine = (*line);
  tempLine.position += lineTransform->GetPosition();
    
  return CheckLineToCube(tempLine, aPair.mBodies[1]->GetOwner()->GET<Transform>(), aPair.mShapes[1]);
}

/**
 * @brief Line to Triangle check
 * @param aPair
 * @return true if collided
 */
bool CollisionChecker::CheckLineToTriangle(CollisionPair &aPair)
{
  // TODO
  return false;
}

/**
 * @brief Line to Line check
 * @param aPair
 * @return true if collided
 */
bool CollisionChecker::CheckLineToLine(CollisionPair &aPair)
{
  // TODO
  return false;
}

/**
 * @brief Line to sphere check
 * @param aSegment Line
 * @param aSphere Sphere transform
 * @param aShape Sphere shape
 */
bool CollisionChecker::CheckLineToSphere(Line const &aSegment, Transform *aSphere, Shape* aShape)
{
  float radius = aShape->GetSize(0);
  Vector3 closestPoint = aSegment.ClosestPointToPoint(aSphere->GetPosition() + aShape->position);
  return (closestPoint - (aSphere->GetPosition() + aShape->position)).length() < radius;
}

/**
 * @brief Line to cube check
 * @param aSegment Line
 * @param aSphere Cube transform
 * @param aShape Cube shape
 */
bool CollisionChecker::CheckLineToCube(Line const &aSegment, Transform *aCube, Shape* aShape)
{
  // TODO MAY BE BROKEN!
  // http://bit.ly/1p1SX3G
  const int RIGHT = 0;
  const int LEFT = 1;
  const int CENTER = 2;
  const float MINIMUM_SIZE = 0.001f;
  bool inside = true;
  int quadrant[3];
  int whichPlane;
  float maxT[3];
  float candidatePlane[3];
  float dimensions = 3;
  Vector3 position = aSegment.position;
  Vector3 direction = aSegment.direction;
  Vector3 min = Vector3(aCube->GetPosition().x + aShape->position.x - aShape->GetSize(0),
                        aCube->GetPosition().y + aShape->position.y - aShape->GetSize(1),
                        aCube->GetPosition().z + aShape->position.z - aShape->GetSize(2));
  Vector3 max = Vector3(aCube->GetPosition().x + aShape->position.x + aShape->GetSize(0),
                        aCube->GetPosition().y + aShape->position.y + aShape->GetSize(1),
                        aCube->GetPosition().z + aShape->position.z + aShape->GetSize(2));
  Vector3 collision;
  
  if(fabs(aShape->GetSize(2)) <= MINIMUM_SIZE)
  {
    dimensions = 2;
  }

  /* Find candidate planes; this loop can be avoided if
      rays cast all from the eye(assume perspective view) */
  for(int i = 0; i < dimensions; ++i)
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
  for(int i = 0; i < dimensions; ++i)
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
  for(int i = 1; i < dimensions; ++i)
  {
    if(maxT[whichPlane] < maxT[i])
    {
      whichPlane = i;
    }
  }

  /* Check final candidate actually inside box */
  if(maxT[whichPlane] < 0) return false;
  for(int i = 0; i < dimensions; ++i)
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

  // Line origin inside cube
  if(inside)
  {
    collision = aSegment.position;
    return true;
  }

  /* Calculate T distances to candidate planes */
  for(int i = 0; i < dimensions; ++i)
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
  for(int i = 1; i < dimensions; ++i)
  {
    if(maxT[whichPlane] < maxT[i])
    {
      whichPlane = i;
    }
  }

  /* Check final candidate actually inside box */
  if(maxT[whichPlane] < 0) return false;
  for(int i = 0; i < dimensions; ++i)
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

