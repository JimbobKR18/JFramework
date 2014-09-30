#ifndef __JFramework_Potential_Pair_h_
#define __JFramework_Potential_Pair_h_

// Forward declaration
class PhysicsObject;

struct PotentialPair
{
  PhysicsObject* mBodies[2];
  
  PotentialPair(PhysicsObject* mBody1, PhysicsObject* mBody2);
  
  void operator=(PotentialPair const &rhs);
  bool operator==(PotentialPair const &rhs);
};

#endif