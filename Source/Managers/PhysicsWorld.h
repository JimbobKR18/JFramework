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
  std::vector<PhysicsObject*> mObjects;
  ForceRegistry               mRegistry;
  GravityGenerator            mGravity;
  Resolver                    mResolver;

  static unsigned const sUID;

public:
  PhysicsWorld(GameApp *aApp);
  virtual ~PhysicsWorld();

  // Object Management
  PhysicsObject*                CreateObject();
  void                          DeleteObject(PhysicsObject *aObject);
  void                          AddObject(PhysicsObject *aObject);
  void                          RemoveObject(PhysicsObject *aObject);
  void                          ClearObjects();
  // Derived from Manager
  virtual void                  Update();
  virtual void                  SendMessage(Message const &aMessage);
  virtual void                  ProcessDelayedMessage(Message *aMessage);
  static std::string            GetName() {return "PhysicsWorld";}
  static unsigned               GetUID() {return sUID;}
  static void                   SerializeLUA();
  // Forces
  void                          RegisterForce(PhysicsObject *aObject, ForceGenerator *aGenerator);
  void                          UnregisterForce(PhysicsObject *aObject, ForceGenerator *aGenerator);
  void                          UnregisterGravity(PhysicsObject *aObject);
  // Misc.
  bool                          LineCollidesWithAnything(Line const &aLine, std::vector<PhysicsObject*> const &aIgnoreList);
  
private:
  void                          SortOnAxis();
  void                          SweepAndPrune();
  
public:
  typedef std::vector<PhysicsObject*>::iterator PhysicsIT;
  typedef std::vector<PhysicsObject*>::const_iterator PhysicsConstIT;
};

#endif /* defined(__JFramework__PhysicsWorld__) */
