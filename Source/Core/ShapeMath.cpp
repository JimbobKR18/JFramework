#include "Common.h"
#include "ShapeMath.h"

namespace ShapeMath
{
  /**
   * @brief Get closest point on line segment, given a point.
   * @param aPoint Point to check.
   * @param aA Start of line.
   * @param aB End of line.
   * @return Closest point along line.
   */
  Vector3 const ClosestPointPointSegment(Vector3 const &aPoint, Vector3 const &aA, Vector3 const &aB)
  {
    /**
      Vector3 end = position + (direction * length);
      Vector3 x1 = position - aPoint;
      Vector3 x2 = end - position;
      float lengthSquared = length * length;
      float t = -(x1.Dot(x2)) / lengthSquared;
      
      if(t > 1.0f)
        t = 1.0f;
      else if(t < 0.0f)
        t = 0.0f;
      
      return position + (direction * (length * t));
   */
    Vector3 ab = aB - aA;
    float t = (aPoint - aA).Dot(ab) / ab.Dot(ab);
    
    if(t < 0)
      t = 0;
    else if(t > 1)
      t = 1;
      
    return aA + (ab * t);
  }
  
  /**
   * @brief Get closest point along AABB, given a point.
   * @param aPoint Point to check.
   * @param aMin Minimum boundary of AABB.
   * @param aMax Maximum boundary of AABB.
   * @return Closest point along AABB.
   */
  Vector3 const ClosestPointPointAABB(Vector3 const &aPoint, Vector3 const &aMin, Vector3 const &aMax)
  {
    Vector3 ret;
    for(int i = 0; i < 3; ++i)
    {
      float v = aPoint.GetValue(i);
      if(v < aMin.GetValue(i)) v = aMin.GetValue(i);
      if(v > aMax.GetValue(i)) v = aMax.GetValue(i);
      ret[i] = v;
    }
    return ret;
  }
  
  /**
   * @brief Get closest point along OBB, given a point.
   * @param aPoint Point to check.
   * @param aCenter Center of OBB.
   * @param aOrientation Orientating vectors of OBB.
   * @param aExtent Max extents of OBB along orienting vectors.
   * @return Closest point along OBB.
   */
  Vector3 const ClosestPointPointOBB(Vector3 const &aPoint, Vector3 const &aCenter, Vector3 const aOrientation[3], Vector3 const &aExtent)
  {
    Vector3 ret;
    Vector3 d = aPoint - aCenter;
    ret = aCenter;
    for(int i = 0; i < 3; ++i)
    {
      float dist = d.Dot(aOrientation[i]);
      if(dist > aExtent.GetValue(i)) dist = aExtent.GetValue(i);
      if(dist < -aExtent.GetValue(i)) dist = -aExtent.GetValue(i);
      ret += aOrientation[i]* dist;
    }
    return ret;
  }
  
  /**
   * @brief Get closest point along triangle, given a point.
   * @param aPoint Point to check.
   * @param aA Point of triangle.
   * @param aB Point of triangle.
   * @param aC Point of triangle.
   * @return Closest point along triangle.
   */
  Vector3 const ClosestPointPointTriangle(Vector3 const &aPoint, Vector3 const &aA, Vector3 const &aB, Vector3 const &aC)
  {
    Vector3 ab = aB - aA;
    Vector3 ac = aC - aA;
    Vector3 bc = aC - aB;
    
    float snom = (aPoint - aA).Dot(ab), sdenom = (aPoint - aB).Dot(aA - aB);
    float tnom = (aPoint - aA).Dot(ac), tdenom = (aPoint - aC).Dot(aA - aC);
    if(snom <= 0.0f && tnom <= 0.0f) 
      return aA;
    
    float unom = (aPoint - aB).Dot(bc), udenom = (aPoint - aC).Dot(aB - aC);
    if(sdenom <= 0.0f && unom <- 0.0f)
      return aB;
    if(tdenom <= 0.0f && udenom <= 0.0f)
      return aC;
    
    Vector3 n = ab.Cross(ac);
    float vc = n.Dot((aA - aPoint).Cross(aB - aPoint));
    if(vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
      return aA + ab * snom / (snom + sdenom);
    
    float va = n.Dot((aB - aPoint).Cross(aC - aPoint));
    if(va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
      return aB + bc * unom / (unom + udenom);
      
    float vb = n.Dot((aC - aPoint).Cross(aA - aPoint));
    if(vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
      return aA + ac * tnom / (tnom + tdenom);
      
    float u = va / (va + vb + vc);
    float v = vb / (va + vb + vc);
    float w = 1.0f - u - v;
    return aA * u + aB * v + aC * w;
  }
  
  /**
   * @brief Get area of triangle.
   * @param aA Point of triangle.
   * @param aB Point of triangle.
   * @param aC Point of triangle.
   * @return Area of triangle.
   */
  float const Signed2DTriArea(Vector3 const &aA, Vector3 const &aB, Vector3 const &aC)
  {
    return (aA.x - aC.x) * (aB.y - aC.y) - (aA.y - aC.y) * (aB.x - aC.x);
  }
}