/*
 * CollisionChecker.h
 *
 *  Created on: Sep 16, 2014
 *      Author: jimbob
 */

#ifndef COLLISIONCHECKER_H_
#define COLLISIONCHECKER_H_

#include "CollisionPair.h"
#include "MathExt.h"
#include "Transform.h"

class CollisionChecker
{
public:
	// Check if objects are actually colliding.
	static bool CheckShapeCollision(CollisionPair &aPair);
	static bool CheckLineCollision(Line const &aSegment, PhysicsObject *aBody);

private:
	// Shape specific functions that shouldn't be visible.
	static bool CheckSphereToSphere(CollisionPair &aPair);
  static bool CheckSphereToCube(CollisionPair &aPair);
  static bool CheckCubeToCube(CollisionPair &aPair);

  // Line functions, also not visible.
  static bool CheckLineToSphere(Line const &aSegment, Transform *aSphere);
  static bool CheckLineToCube(Line const &aSegment, Transform *aCube);
};

#endif /* COLLISIONCHECKER_H_ */
