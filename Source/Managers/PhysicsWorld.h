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
#include "Manager.h"
#include "MathExt.h"
#include "ForceRegistry.h"
#include "GravityGenerator.h"
#include "Resolver.h"

class PhysicsObject;

class PhysicsWorld : public Manager
{
private:
  std::vector<PhysicsObject *> mObjects;
  ForceRegistry mRegistry;
  GravityGenerator mGravity;
  Resolver mResolver;

public:
  PhysicsWorld(GameApp *aApp);
  virtual ~PhysicsWorld();

  PhysicsObject *CreateObject();
  void DeleteObject(PhysicsObject *aObject);
  void ClearObjects();

  virtual void Update();
  virtual void SendMessage(Message const &aMessage);
  static std::string GetName() {return "PhysicsWorld";}

  void RegisterForce(PhysicsObject *aObject, ForceGenerator *aGenerator);
  void UnregisterForce(PhysicsObject *aObject, ForceGenerator *aGenerator);

private:
  void AddObject(PhysicsObject *aObject);
  void RemoveObject(PhysicsObject *aObject);
  std::vector<PhysicsObject*> SortOnAxis();
  void SweepAndPrune(std::vector<PhysicsObject*> aSortedObjects);
};

#endif /* defined(__JFramework__PhysicsWorld__) */
