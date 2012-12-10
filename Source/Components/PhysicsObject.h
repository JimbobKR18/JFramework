//
//  PhysicsObject.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#ifndef __JFramework__PhysicsObject__
#define __JFramework__PhysicsObject__

#include "Component.h"
#include "PhysicsWorld.h"
#include "MathExt.h"

class PhysicsObject : public Component
{
private:
  PhysicsWorld *mWorld;
  Vector3 mVelocity, mAcceleration, mForces, mBroadSize;
  float mMass, mInverseMass;
  float mDamping;
  bool mStatic;

public:
  enum {SPHERE, CUBE} mShape;

  PhysicsObject(PhysicsWorld *aWorld);
  virtual ~PhysicsObject();

  // Methods from Component
  void Update();
  void SendMessage(Message const &aMessage);
  void ReceiveMessage(Message const &aMessage);
  static std::string GetName() {return "PhysicsObject";}

  // Methods - Misc.
  void AddForce(Vector3 const &aForce);

  // Getters and setters
  Vector3 GetVelocity() const;
  void SetVelocity(Vector3 const &aVel);
  void SetAcceleration(Vector3 const &aAccel);

  float GetMass() const;
  void SetMass(float aMass);

  bool IsStatic();
  void SetStatic(bool aStatic);

  Vector3 GetBroadSize() const;
};

#endif /* defined(__JFramework__PhysicsObject__) */
