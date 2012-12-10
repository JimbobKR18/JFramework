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

	void Update();
	void AddPrelimPair(CollisionPair const &aPair);
	void AddPair(CollisionPair const &aPair);
	bool Find(PhysicsObject *aObject1, PhysicsObject *aObject2);

private:
	// Methods to resolve collisions
	void ResolvePenetration(CollisionPair const &aPair);
	void ResolveVelocity(CollisionPair const &aPair);
	void Resolve(CollisionPair &aPair);

	// Check if objects are actually colliding
	void CheckCollision(CollisionPair &aPair);
	bool CheckSphereToSphere(CollisionPair &aPair);
  bool CheckSphereToCube(CollisionPair &aPair);
  bool CheckCubeToCube(CollisionPair &aPair);

  // Calculation of normals etc.
	void CalculateSphereToSphere(CollisionPair &aPair);
	void CalculateSphereToCube(CollisionPair &aPair);
	void CalculateCubeToCube(CollisionPair &aPair);
};

#endif
