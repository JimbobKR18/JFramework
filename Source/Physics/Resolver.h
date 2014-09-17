#ifndef __JFramework_Resolver_h_
#define __JFramework_Resolver_h_

#include <list>
#include "CollisionPair.h"

class Resolver
{
private:
	std::list<CollisionPair> mPairs;
	std::list<CollisionPair> mBroadPairs;

public:
	Resolver();
	~Resolver();

	void Update(float aDuration);
	void AddPrelimPair(CollisionPair const &aPair);
	void AddPair(CollisionPair const &aPair);
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
	void  CalculateSphereToCube(CollisionPair &aPair);
	void  CalculateCubeToCube(CollisionPair &aPair);
};

#endif
