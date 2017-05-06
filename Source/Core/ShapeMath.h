#ifndef __JFramework_ShapeMath_h_
#define __JFramework_ShapeMath_h_

#include "MathExt.h"
#include "Shape.h"

namespace ShapeMath
{
  Vector3 const ClosestPointPointSegment(Vector3 const &aPoint, Vector3 const &aA, Vector3 const &aB);
  Vector3 const ClosestPointPointAABB(Vector3 const &aPoint, Vector3 const &aMin, Vector3 const &aMax);
  Vector3 const ClosestPointPointOBB(Vector3 const &aPoint, Vector3 const &aCenter, Vector3 const aOrientation[3], Vector3 const &aExtent);
  Vector3 const ClosestPointPointTriangle(Vector3 const &aPoint, Vector3 const &aA, Vector3 const &aB, Vector3 const &aC);
}

#endif