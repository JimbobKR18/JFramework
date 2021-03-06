/*
 * CollisionChecker.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: jimbob
 */

#include "CollisionChecker.h"
#include "PhysicsObject.h"
#include "ShapeMath.h"

/**
 * @brief General catch all shape collision caller.
 * @param aPair
 */
std::vector<CollisionPair> CollisionChecker::CheckShapeCollision(PotentialPair const &aPair)
{
  std::vector<CollisionPair> pairs;
  PhysicsObject::ConstShapeIT it1End = aPair.mBodies[0]->GetShapes().end();
  PhysicsObject::ConstShapeIT it2End = aPair.mBodies[1]->GetShapes().end();
  for(PhysicsObject::ConstShapeIT it = aPair.mBodies[0]->GetShapes().begin();
      it != it1End; ++it)
  {
    for(PhysicsObject::ConstShapeIT it2 = aPair.mBodies[1]->GetShapes().begin();
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
        case Shape::AABB:
          collided = CheckSphereToAABB(potentialPair);
          break;
        case Shape::TRIANGLE:
          collided = CheckTriangleToSphere(potentialPair);
          break;
        case Shape::LINE:
          collided = CheckLineToSphere(potentialPair);
          break;
        case Shape::OBB:
          collided = CheckOBBToSphere(potentialPair);
          break;
        default:
          break;
        }
        break;
      case Shape::AABB:
        switch((*it2)->shape)
        {
        case Shape::SPHERE:
          collided = CheckSphereToAABB(potentialPair);
          break;
        case Shape::AABB:
          collided = CheckAABBToAABB(potentialPair);
          break;
        case Shape::TRIANGLE:
          collided = CheckTriangleToAABB(potentialPair);
          break;
        case Shape::LINE:
          collided = CheckLineToAABB(potentialPair);
          break;
        case Shape::OBB:
          collided = CheckOBBToAABB(potentialPair);
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
        case Shape::AABB:
          collided = CheckTriangleToAABB(potentialPair);
          break;
        case Shape::TRIANGLE:
          collided = CheckTriangleToTriangle(potentialPair);
          break;
        case Shape::LINE:
          collided = CheckLineToTriangle(potentialPair);
          break;
        case Shape::OBB:
          collided = CheckOBBToTriangle(potentialPair);
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
        case Shape::AABB:
          collided = CheckLineToAABB(potentialPair);
          break;
        case Shape::TRIANGLE:
          collided = CheckLineToTriangle(potentialPair);
          break;
        case Shape::LINE:
          collided = CheckLineToLine(potentialPair);
          break;
        case Shape::OBB:
          collided = CheckOBBToLine(potentialPair);
          break;
        default:
          break;
        }
        break;
      case Shape::OBB:
        switch((*it2)->shape)
        {
        case Shape::SPHERE:
          collided = CheckOBBToSphere(potentialPair);
          break;
        case Shape::AABB:
          collided = CheckOBBToAABB(potentialPair);
          break;
        case Shape::TRIANGLE:
          collided = CheckOBBToTriangle(potentialPair);
          break;
        case Shape::LINE:
          collided = CheckOBBToLine(potentialPair);
          break;
        case Shape::OBB:
          collided = CheckOBBToOBB(potentialPair);
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
  case Shape::AABB:
    return CheckLineToAABB(aSegment, aTransform, aShape);
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

  Vector3 t1Pos = ShapeMath::GetLocalCoordinates(t1, aPair.mShapes[0]->position);
  Vector3 t2Pos = ShapeMath::GetLocalCoordinates(t2, aPair.mShapes[1]->position);
  float t1Size = aPair.mShapes[0]->GetSize(0) * t1->GetHierarchicalScale().x;
  float t2Size = aPair.mShapes[1]->GetSize(0) * t2->GetHierarchicalScale().x;

  if(t1Size + t2Size > (t1Pos - t2Pos).length())
  {
    return true;
  }

  return false;
}

/**
 * @brief Sphere to aabb check
 * @param aPair
 */
bool CollisionChecker::CheckSphereToAABB(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape != Shape::SPHERE)
    aPair.Switch();

  Transform *sphere = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *aabb = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  Vector3 spherePos = ShapeMath::GetLocalCoordinates(sphere, aPair.mShapes[0]->position);
  Vector3 aabbPos = ShapeMath::GetLocalCoordinates(aabb, aPair.mShapes[1]->position);
  Vector3 aabbSize = dynamic_cast<AxisAlignedBoundingBox*>(aPair.mShapes[1])->size.Multiply(aabb->GetHierarchicalScale());
  Vector3 closestPoint = ShapeMath::ClosestPointPointAABB(spherePos, aabbPos - aabbSize, aabbPos + aabbSize);

  Vector3 dist = spherePos - closestPoint;
  float size = aPair.mShapes[0]->GetSize(0) * sphere->GetHierarchicalScale().x;
  return dist.Dot(dist) <= size * size;
}

/**
 * @brief AABB to aabb check
 * @param aPair
 */
bool CollisionChecker::CheckAABBToAABB(CollisionPair &aPair)
{
  Transform *t1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *t2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  Vector3 t1Pos = ShapeMath::GetLocalCoordinates(t1, aPair.mShapes[0]->position);
  Vector3 t2Pos = ShapeMath::GetLocalCoordinates(t2, aPair.mShapes[1]->position);

  bool xCheck = fabs(t1Pos.x - t2Pos.x) <= aPair.mShapes[0]->GetSize(0) * t1->GetHierarchicalScale().x + aPair.mShapes[1]->GetSize(0) * t2->GetHierarchicalScale().x;
  bool yCheck = fabs(t1Pos.y - t2Pos.y) <= aPair.mShapes[0]->GetSize(1) * t1->GetHierarchicalScale().y + aPair.mShapes[1]->GetSize(1) * t2->GetHierarchicalScale().y;
  bool zCheck = fabs(t1Pos.z - t2Pos.z) <= aPair.mShapes[0]->GetSize(2) * t1->GetHierarchicalScale().z + aPair.mShapes[1]->GetSize(2) * t2->GetHierarchicalScale().z;

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
  Vector3 spherePos = ShapeMath::GetLocalCoordinates(sphereTransform, sphere->position);
  Vector3 a = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(0) + triangle->position);
  Vector3 b = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(1) + triangle->position);
  Vector3 c = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(2) + triangle->position);
  Vector3 closestPoint = ShapeMath::ClosestPointPointTriangle(spherePos, a, b, c);
  
  Vector3 dist = closestPoint - spherePos;
  float size = sphere->GetSize(0) * sphereTransform->GetHierarchicalScale().x;
  if(dist.Dot(dist) <= size * size)
    return true;
  
  return false;
}

/**
 * @brief Triangle to AABB check
 * @param aPair
 */
bool CollisionChecker::CheckTriangleToAABB(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::AABB)
    aPair.Switch();
  
  Triangle* triangle = (Triangle*)aPair.mShapes[0];
  AxisAlignedBoundingBox* aabb = (AxisAlignedBoundingBox*)aPair.mShapes[1];
  Transform* triTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* aabbTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 aabbPos = ShapeMath::GetLocalCoordinates(aabbTransform, aabb->position);
  Vector3 a = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(0) + triangle->position);
  Vector3 b = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(1) + triangle->position);
  Vector3 c = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(2) + triangle->position);
  Vector3 closestPoint = ShapeMath::ClosestPointPointTriangle(aabbPos, a, b, c);
  Vector3 dist = closestPoint - aabbPos;
  
  for(int i = 0; i < 3; ++i)
  {
    if(fabs(dist[i]) > aabb->GetSize(i) * aabbTransform->GetHierarchicalScale().GetValue(i))
      return false;
  }
    
  return true;
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
  tempLine.direction = tempLine.direction.Multiply(lineTransform->GetRotation() * lineTransform->GetHierarchicalScale());
  tempLine.position = ShapeMath::GetLocalCoordinates(lineTransform, tempLine.position);
    
  return CheckLineToSphere(tempLine, aPair.mBodies[1]->GetOwner()->GET<Transform>(), aPair.mShapes[1]);
}

/**
 * @brief Line to AABB check
 * @param aPair
 * @return true if collided
 */
bool CollisionChecker::CheckLineToAABB(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::AABB)
    aPair.Switch();
    
  // Move line into world space
  // This is a bit slow since it makes a new line every time.
  Transform *lineTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Line* line = (Line*)(aPair.mShapes[0]);
  Line tempLine = (*line);
  tempLine.direction = tempLine.direction.Multiply(lineTransform->GetRotation() * lineTransform->GetHierarchicalScale());
  tempLine.position = ShapeMath::GetLocalCoordinates(lineTransform, tempLine.position);
    
  return CheckLineToAABB(tempLine, aPair.mBodies[1]->GetOwner()->GET<Transform>(), aPair.mShapes[1]);
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
  Transform *t1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *t2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  Line *l1 = dynamic_cast<Line*>(aPair.mShapes[0]);
  Line *l2 = dynamic_cast<Line*>(aPair.mShapes[1]);
  
  Vector3 a = ShapeMath::GetLocalCoordinates(t1, l1->position);
  Vector3 b = a + (l1->direction.Multiply(t1->GetHierarchicalRotation() * t1->GetHierarchicalScale()) * l1->length);
  Vector3 c = ShapeMath::GetLocalCoordinates(t2, l2->position);
  Vector3 d = c + (l2->direction.Multiply(t2->GetHierarchicalRotation() * t2->GetHierarchicalScale()) * l2->length);
  
  float a1 = ShapeMath::Signed2DTriArea(a, b, d);
  float a2 = ShapeMath::Signed2DTriArea(a, b, c);
  
  if(a1 * a2 < 0)
  {
    float a3 = ShapeMath::Signed2DTriArea(c, d, a);
    float a4 = a3 + a2 - a1;
    
    if(a3 * a4 < 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * @brief OBB to Sphere check
 * @param aPair
 * @return true if collided
 */
bool CollisionChecker::CheckOBBToSphere(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::SPHERE)
    aPair.Switch();
  OrientedBoundingBox *obb = dynamic_cast<OrientedBoundingBox*>(aPair.mShapes[0]);
  Sphere *sphere = dynamic_cast<Sphere*>(aPair.mShapes[1]);
  Transform* obbTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* sphereTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  Vector3 orientation[3];
  orientation[0] = obbTransform->GetHierarchicalRotation() * obb->right;
  orientation[1] = obbTransform->GetHierarchicalRotation() * obb->up;
  orientation[2] = obbTransform->GetHierarchicalRotation() * obb->forward;
  Vector3 spherePos = ShapeMath::GetLocalCoordinates(sphereTransform, sphere->position);
  Vector3 obbPos = ShapeMath::GetLocalCoordinates(obbTransform, obb->position);
  Vector3 closestPoint = ShapeMath::ClosestPointPointOBB(spherePos, obbPos, orientation, obbTransform->GetHierarchicalScale().Multiply(obb->extents));
  Vector3 diff = closestPoint - spherePos;
  
  float size = sphere->radius * sphereTransform->GetHierarchicalScale().x;
  return diff.Dot(diff) <= size * size;
}

/**
 * @brief OBB to AABB check
 * @param aPair
 * @return true if collided
 */
bool CollisionChecker::CheckOBBToAABB(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::AABB)
    aPair.Switch();
  OrientedBoundingBox *obb = dynamic_cast<OrientedBoundingBox*>(aPair.mShapes[0]);
  AxisAlignedBoundingBox *aabb = dynamic_cast<AxisAlignedBoundingBox*>(aPair.mShapes[1]);
  Transform* obbTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* aabbTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  Vector3 orientation[3];
  orientation[0] = obbTransform->GetHierarchicalRotation() * obb->right;
  orientation[1] = obbTransform->GetHierarchicalRotation() * obb->up;
  orientation[2] = obbTransform->GetHierarchicalRotation() * obb->forward;
  Vector3 aabbPos = ShapeMath::GetLocalCoordinates(aabbTransform, aabb->position);
  Vector3 obbPos = ShapeMath::GetLocalCoordinates(obbTransform, obb->position);
  Vector3 closestPoint = ShapeMath::ClosestPointPointOBB(aabbPos, obbPos, orientation, obbTransform->GetHierarchicalScale().Multiply(obb->extents));
  Vector3 diff = closestPoint - aabbPos;
  
  for(int i = 0; i < 3; ++i)
  {
    if(fabs(diff[i]) > aabb->GetSize(i) * aabbTransform->GetHierarchicalScale().GetValue(i))
      return false;
  }
    
  return true;
}

/**
 * @brief OBB to OBB check
 * @param aPair
 * @return true if collided
 */
bool CollisionChecker::CheckOBBToOBB(CollisionPair &aPair)
{
  OrientedBoundingBox *obb1 = dynamic_cast<OrientedBoundingBox*>(aPair.mShapes[0]);
  OrientedBoundingBox *obb2 = dynamic_cast<OrientedBoundingBox*>(aPair.mShapes[1]);
  Transform* obb1Transform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* obb2Transform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  Vector3 orientation[3];
  orientation[0] = obb2Transform->GetHierarchicalRotation() * obb2->right;
  orientation[1] = obb2Transform->GetHierarchicalRotation() * obb2->up;
  orientation[2] = obb2Transform->GetHierarchicalRotation() * obb2->forward;
  Vector3 obb1Pos = ShapeMath::GetLocalCoordinates(obb1Transform, obb1->position);
  Vector3 obb2Pos = ShapeMath::GetLocalCoordinates(obb2Transform, obb2->position);
  Vector3 closestPoint = ShapeMath::ClosestPointPointOBB(obb1Pos, obb2Pos, orientation, obb2Transform->GetHierarchicalScale().Multiply(obb2->extents));
  Vector3 diff = closestPoint - obb1Pos;
  
  for(int i = 0; i < 3; ++i)
  {
    float compare = obb1->extents[i] * obb1Transform->GetHierarchicalScale().GetValue(i);
    float d = diff.Dot(obb1Transform->GetHierarchicalRotation() * obb1->GetAxis(i));
    if(fabs(d) > compare)
      return false;
  }
    
  return true;
}

/**
 * @brief OBB to Triangle check
 * @param aPair
 * @return true if collided
 */
bool CollisionChecker::CheckOBBToTriangle(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::TRIANGLE)
    aPair.Switch();
  
  OrientedBoundingBox *obb = dynamic_cast<OrientedBoundingBox*>(aPair.mShapes[0]);
  Triangle *triangle = dynamic_cast<Triangle*>(aPair.mShapes[1]);
  Transform* obbTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* triTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  Vector3 a = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(0) + triangle->position);
  Vector3 b = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(1) + triangle->position);
  Vector3 c = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(2) + triangle->position);
  Vector3 obbPos = ShapeMath::GetLocalCoordinates(obbTransform, obb->position);
  Vector3 closestPoint = ShapeMath::ClosestPointPointTriangle(obbPos, a, b, c);
  Vector3 diff = closestPoint - obbPos;
  
  for(int i = 0; i < 3; ++i)
  {
    float compare = obb->extents[i] * obbTransform->GetHierarchicalScale().GetValue(i);
    float d = diff.Dot(obbTransform->GetHierarchicalRotation() * obb->GetAxis(i));
    if(fabs(d) > compare)
      return false;
  }
    
  return true;
}

/**
 * @brief OBB to Line check
 * @param aPair
 * @return true if collided
 */
bool CollisionChecker::CheckOBBToLine(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::LINE)
    aPair.Switch();
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
  float radius = aShape->GetSize(0) * aSphere->GetHierarchicalScale().x;
  Vector3 scaledSpherePos = ShapeMath::GetLocalCoordinates(aSphere, aShape->position);
  Vector3 lineEnd = aSegment.position + (aSegment.direction * aSegment.length);
  Vector3 closestPoint = ShapeMath::ClosestPointPointSegment(scaledSpherePos, aSegment.position, lineEnd);
  return (closestPoint - scaledSpherePos).length() < radius;
}

/**
 * @brief Line to aabb check
 * @param aSegment Line
 * @param aSphere AABB transform
 * @param aShape AABB shape
 */
bool CollisionChecker::CheckLineToAABB(Line const &aSegment, Transform *aAABB, Shape* aShape)
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
  Vector3 min = Vector3(aShape->position.x - aShape->GetSize(0),
                        aShape->position.y - aShape->GetSize(1),
                        aShape->position.z - aShape->GetSize(2));
  Vector3 max = Vector3(aShape->position.x + aShape->GetSize(0),
                        aShape->position.y + aShape->GetSize(1),
                        aShape->position.z + aShape->GetSize(2));
                        
  min = ShapeMath::GetLocalCoordinates(aAABB, min);
  max = ShapeMath::GetLocalCoordinates(aAABB, max);
  Vector3 collision;
  
  if(fabs(aShape->GetSize(2) * aAABB->GetHierarchicalScale().GetValue(2)) <= MINIMUM_SIZE)
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

  // Line origin inside aabb
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

  // Line origin inside aabb
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

