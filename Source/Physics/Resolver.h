#ifndef __JFramework_Resolver_h_
#define __JFramework_Resolver_h_

#include <list>
#include "CollisionPair.h"

class Resolver
{
private:
	std::list<CollisionPair> mPairs;

public:
	Resolver();
	~Resolver();

	void Update();
	void AddPair(CollisionPair const &aPair);
	bool Find(PhysicsObject *aObject1, PhysicsObject *aObject2);

private:
	void ResolvePenetration(CollisionPair const &aPair);
	void ResolveVelocity(CollisionPair const &aPair);
	void Resolve(CollisionPair &aPair);
	void CalculateSphereToSphere(CollisionPair &aPair);
	void CalculateSphereToCube(CollisionPair &aPair);
	void CalculateCubeToCube(CollisionPair &aPair);
};

#endif
