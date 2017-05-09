#ifndef __JFramework_Resolver_h_
#define __JFramework_Resolver_h_

#include <list>
#include "CollisionPair.h"
#include "PotentialPair.h"

class Resolver
{
private:
  std::list<CollisionPair> mCollidedPairs;
  std::unordered_map<size_t, PotentialPair> mPotentialPairs;

public:
  Resolver();
  ~Resolver();

  void Update(float aDuration);
  void AddPrelimPair(PotentialPair const &aPair);
  void AddCollidedPair(CollisionPair const &aPair);
  bool Find(PhysicsObject *aObject1, PhysicsObject *aObject2);

private:
  // Methods to resolve collisions
  float CalculateSeparatingVelocity(CollisionPair const &aPair);
  void  ResolvePenetration(CollisionPair const &aPair);
  void  ResolveVelocity(CollisionPair const &aPair, float aDuration);
  void  SendCollisionMessages(CollisionPair &aPair) const;
  void  Resolve(CollisionPair &aPair, float aDuration);

  // Calculation of normals etc.
  void  CalculateSphereToSphere(CollisionPair &aPair);
  void  CalculateSphereToAABB(CollisionPair &aPair);
  void  CalculateAABBToAABB(CollisionPair &aPair);
  void  CalculateTriangleToSphere(CollisionPair &aPair);
  void  CalculateTriangleToAABB(CollisionPair &aPair);
  void  CalculateTriangleToTriangle(CollisionPair &aPair);
  void  CalculateLineToSphere(CollisionPair &aPair);
  void  CalculateLineToAABB(CollisionPair &aPair);
  void  CalculateLineToTriangle(CollisionPair &aPair);
  void  CalculateLineToLine(CollisionPair &aPair);
  void  CalculateOBBToSphere(CollisionPair &aPair);
  void  CalculateOBBToAABB(CollisionPair &aPair);
  void  CalculateOBBToOBB(CollisionPair &aPair);
  void  CalculateOBBToTriangle(CollisionPair &aPair);
  void  CalculateOBBToLine(CollisionPair &aPair);
  
  // Hash calculations
  size_t CalculateHash(PhysicsObject *aObject1, PhysicsObject *aObject2);
};

#endif
