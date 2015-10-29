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
#include "Shape.h"

class PhysicsObject : public Component
{
private:
  PhysicsWorld*             mWorld;
  Vector3                   mVelocity,
                            mAcceleration,
                            mForces,
                            mBroadSize;
  float                     mMass,
                            mInverseMass;
  float                     mDamping;
  bool                      mStatic,
                            mGravity,
                            mPassable;
  std::vector<std::string>  mIgnoreList;
  std::vector<Shape*>       mShapes;
  
  static int const sUID;

public:
  typedef std::vector<Shape*>::iterator shapeIT;
  typedef std::vector<Shape*>::const_iterator constShapeIT;

  PhysicsObject(PhysicsWorld *aWorld);
  virtual ~PhysicsObject();

  // Methods from Component
  void                 Update();
  void                 SendMessage(Message const &aMessage);
  void                 ReceiveMessage(Message const &aMessage);
  void                 Serialize(Parser &aParser);
  void                 Deserialize(Parser &aParser);
  static int           GetUID() {return sUID;}

  // Methods - Misc.
  void                 AddShape(Shape* aShape);
  void                 AddForce(Vector3 const &aForce);
  void                 AddIgnore(std::string const &aObjectName);
  bool                 IgnoreObject(std::string const &aObjectName);

  // Getters and setters
  Vector3              GetVelocity() const;
  Vector3              GetAcceleration() const;
  void                 SetVelocity(Vector3 const &aVel);
  void                 SetAcceleration(Vector3 const &aAccel);

  float                GetMass() const;
  void                 SetMass(float aMass);
  
  float                GetDamping() const;
  void                 SetDamping(float aDamping);

  bool                 IsStatic() const;
  void                 SetStatic(bool aStatic);
  
  bool                 IsAffectedByGravity() const;
  void                 SetAffectedByGravity(bool aGravity);
  
  bool                 IsPassable() const;
  void                 SetPassable(bool aPassable);

  Vector3              GetBroadSize() const;
  void                 SetBroadSize(Vector3 const &aSize);
  
  std::vector<Shape*>& GetShapes();
};

#endif /* defined(__JFramework__PhysicsObject__) */
