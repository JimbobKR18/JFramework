#ifndef __JFramework_Potential_Pair_h_
#define __JFramework_Potential_Pair_h_

#include "Common.h"

// Forward declaration
class PhysicsObject;

struct PotentialPair
{
public:
  PhysicsObject* mBodies[2];
  float mDistance;
  unsigned long long mValue;
  
  PotentialPair();
  PotentialPair(PhysicsObject* mBody1, PhysicsObject* mBody2, float aDistance);
  
  void operator=(PotentialPair const &rhs);
  bool operator==(PotentialPair const &rhs) const;
  bool operator<(PotentialPair const &rhs) const;
};

#endif
