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
	void Resolve(CollisionPair const &aPair);

private:
	void SolveSphereToSphere(CollisionPair const &aPair);
	void SolveSphereToCube(CollisionPair const &aPair);
	void SolveCubeToCube(CollisionPair const &aPair);
};

#endif
