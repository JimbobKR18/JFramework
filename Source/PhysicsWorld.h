//
//  PhysicsWorld.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#ifndef __JFramework__PhysicsWorld__
#define __JFramework__PhysicsWorld__

#include "Box2D/Box2D.h"

class PhysicsWorld
{
private:
  b2World *mWorld;
  b2Vec2 mGravity;
  bool mDoSleep;
  
public:
  PhysicsWorld()
  {
    mGravity = b2Vec2(0.0f, -10.0f);
    mDoSleep = true;
    
    mWorld = new b2World(mGravity);
  }
};

#endif /* defined(__JFramework__PhysicsWorld__) */
