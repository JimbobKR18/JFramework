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
public:
  typedef std::unordered_map<int, HashString> IgnoreContainer;
  typedef std::vector<Shape*>::iterator shapeIT;
  typedef std::vector<Shape*>::const_iterator constShapeIT;
  
private:
  PhysicsWorld*       mWorld;
  Vector3             mVelocity,
                      mAcceleration,
                      mForces,
                      mBroadSize;
  float               mMass,
                      mInverseMass;
  float               mDamping,
                      mRestitution,
                      mMaximumVelocity;
  bool                mStatic,
                      mGravity,
                      mPassable,
                      mActive;
  IgnoreContainer     mIgnoreList;
  std::vector<Shape*> mShapes;
  
  static int const sUID;

public:
  PhysicsObject(PhysicsWorld *aWorld);
  virtual ~PhysicsObject();

  // Methods from Component
  virtual void         Update();
  virtual void         SendMessage(Message const &aMessage);
  virtual void         ReceiveMessage(Message const &aMessage);
  virtual void         Serialize(Parser &aParser);
  virtual void         Deserialize(Parser &aParser);
  static void          SerializeLUA();
  static int           GetUID() {return sUID;}

  // Methods - Misc.
  void                 AddShape(Shape* aShape);
  void                 AddForce(Vector3 const &aForce);
  void                 ClearForces();
  void                 AddIgnore(HashString const &aObjectName);
  bool                 IgnoreObject(HashString const &aObjectName);

  // Getters and setters
  Vector3              GetVelocity() const;
  void                 SetVelocity(Vector3 const &aVel);
  Vector3              GetAcceleration() const;
  void                 SetAcceleration(Vector3 const &aAccel);

  float                GetMass() const;
  float                GetInverseMass() const;
  void                 SetMass(float const aMass);
  
  float                GetDamping() const;
  void                 SetDamping(float const aDamping);
  
  float                GetRestitution() const;
  void                 SetRestitution(float const aRestitution);
  
  float                GetMaxVelocity() const;
  void                 SetMaxVelocity(float const aMaximumVelocity); 

  bool                 IsStatic() const;
  void                 SetStatic(bool const aStatic);
  
  bool                 IsAffectedByGravity() const;
  void                 SetAffectedByGravity(bool const aGravity);
  
  bool                 IsPassable() const;
  void                 SetPassable(bool const aPassable);
  
  bool                 IsActive() const;
  void                 SetActive(bool const aActive);

  Vector3              GetBroadSize() const;
  void                 SetBroadSize(Vector3 const &aSize);
  
  IgnoreContainer      GetIgnoreList() const;
  void                 SetIgnoreList(IgnoreContainer const &aIgnoreList);
  
  std::vector<Shape*>& GetShapes();
};

#endif /* defined(__JFramework__PhysicsObject__) */
