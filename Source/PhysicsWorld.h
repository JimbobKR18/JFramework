//
//  PhysicsWorld.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#ifndef __JFramework__PhysicsWorld__
#define __JFramework__PhysicsWorld__

#include "Common.h"
#include "Box2D/Box2D.h"
#include "Manager.h"

class PhysicsObject;

class PhysicsWorld : public Manager
{
private:
  b2World *mWorld;
  b2Vec2 mGravity;
  bool mDoSleep;
  
  std::vector<PhysicsObject *> mObjects;

public:
  PhysicsWorld();
  virtual ~PhysicsWorld();

  PhysicsObject *CreateObject();
  void DeleteObject(PhysicsObject *aObject);
  void ClearObjects();

  b2World *GetWorld();

  virtual void Update();
  virtual void SendMessage(Message const &aMessage);

private:
  void AddObject(PhysicsObject *aObject);
  void RemoveObject(PhysicsObject *aObject);
};

#endif /* defined(__JFramework__PhysicsWorld__) */
