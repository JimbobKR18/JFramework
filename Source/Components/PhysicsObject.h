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
#include "Math.h"

class PhysicsObject : public Component
{
private:
  PhysicsWorld *mWorld;
  Vector3 mVelocity, mForces;

public:
  PhysicsObject(PhysicsWorld *aWorld);
  virtual ~PhysicsObject();

  void Update();
  void SendMessage(Message const &aMessage);
  void ReceiveMessage(Message const &aMessage);

  void AddForce(Vector3 const &rhs);
  void SetVelocity(Vector3 const &rhs);
};

#endif /* defined(__JFramework__PhysicsObject__) */
