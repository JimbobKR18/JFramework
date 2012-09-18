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
#include "Box2D/Box2D.h"

class PhysicsObject : public Component
{
private:
  b2Body *mBody;
  std::vector<b2Joint *> mJoints;
  
<<<<<<< HEAD
  virtual ~PhysicsObject() {}
=======
public:
  PhysicsObject() : Component("PhysicsObject")
  {
    //mBody = b2World::CreateBody()
  }
  virtual ~PhysicsObject();
  
>>>>>>> Forgot some stuff
  void Update();
  void SendMessage(Message const &aMessage);
  void ReceiveMessage(Message const &aMessage);
};

#endif /* defined(__JFramework__PhysicsObject__) */
