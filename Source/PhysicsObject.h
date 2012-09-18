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
#include "Box2D/Box2D.h"

class PhysicsObject : public Component
{
private:
  b2Body *mBody;
  b2BodyDef mBodyDef;
  PhysicsWorld *mWorld;
  std::vector<b2Joint *> mJoints;
  std::vector<b2JointDef *> mJointDefs;
  
public:
  PhysicsObject(PhysicsWorld *aWorld);
  virtual ~PhysicsObject();
  
  void Update();
  void SendMessage(Message const &aMessage);
  void ReceiveMessage(Message const &aMessage);
};

#endif /* defined(__JFramework__PhysicsObject__) */
