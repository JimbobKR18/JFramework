/*
 * CollisionChecker.h
 *
 *  Created on: Sep 16, 2014
 *      Author: jimbob
 */

#ifndef COLLISIONCHECKER_H_
#define COLLISIONCHECKER_H_

#include "CollisionPair.h"
#include "PotentialPair.h"
#include "MathExt.h"
#include "Transform.h"

class CollisionChecker
{
public:
  // Check if objects are actually colliding.
  static std::vector<CollisionPair> CheckShapeCollision(PotentialPair const &aPair);
  static bool CheckLineCollision(Line const &aSegment, Transform* aTransform, Shape* aShape);
  
private:
  // Shape specific functions that shouldn't be visible.
  static bool CheckSphereToSphere(CollisionPair &aPair);
  static bool CheckSphereToAABB(CollisionPair &aPair);
  static bool CheckAABBToAABB(CollisionPair &aPair);
  static bool CheckTriangleToSphere(CollisionPair &aPair);
  static bool CheckTriangleToAABB(CollisionPair &aPair);
  static bool CheckTriangleToTriangle(CollisionPair &aPair);
  static bool CheckLineToSphere(CollisionPair &aPair);
  static bool CheckLineToAABB(CollisionPair &aPair);
  static bool CheckLineToTriangle(CollisionPair &aPair);
  static bool CheckLineToLine(CollisionPair &aPair);

  // Line functions, also not visible.
  static bool CheckLineToSphere(Line const &aSegment, Transform *aSphere, Shape* aShape);
  static bool CheckLineToAABB(Line const &aSegment, Transform *aAABB, Shape* aShape);
};

#endif /* COLLISIONCHECKER_H_ */
