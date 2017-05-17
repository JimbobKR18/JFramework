#ifndef __JFramework_Potential_Pair_h_
#define __JFramework_Potential_Pair_h_

// Forward declaration
class PhysicsObject;

struct PotentialPair
{
  PhysicsObject* mBodies[2];
  float mDistance;
  
  PotentialPair();
  PotentialPair(PhysicsObject* mBody1, PhysicsObject* mBody2, float aDistance);
  
  void operator=(PotentialPair const &rhs);
  bool operator==(PotentialPair const &rhs) const;
  bool operator<(PotentialPair const &rhs) const;
};

#endif