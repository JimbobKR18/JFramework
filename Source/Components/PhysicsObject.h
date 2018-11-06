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
#include "Joint.h"

class PhysicsObject : public Component
{
public:
  typedef std::unordered_map<int, HashString> IgnoreContainer;
  typedef IgnoreContainer::iterator IgnoreIT;
  typedef IgnoreContainer::const_iterator ConstIgnoreIT;
  
  typedef std::vector<Shape*> ShapeContainer;
  typedef ShapeContainer::iterator ShapeIT;
  typedef ShapeContainer::const_iterator ConstShapeIT;
  
  typedef std::vector<Joint*> JointContainer;
  typedef JointContainer::iterator JointIT;
  typedef JointContainer::const_iterator ConstJointIT;
  
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
                      mActive,
                      mPaused;
  IgnoreContainer     mIgnoreList;
  ShapeContainer      mShapes;
  JointContainer      mJoints;
  
  static int const sUID;

public:
  PhysicsObject(PhysicsWorld *aWorld);
  PhysicsObject(PhysicsObject const &aPhysicsObject);
  virtual ~PhysicsObject();

  // Methods from Component
  virtual void         Update();
  virtual void         SendMessage(Message const &aMessage);
  virtual void         ReceiveMessage(Message const &aMessage);
  virtual void         Serialize(ParserNode *aNode);
  virtual void         Deserialize(ParserNode *aNode);
  virtual Component*   Clone(GameObject *aNewOwner) const;
  static void          SerializeLUA();
  static int           GetUID() {return sUID;}

  // Methods - Misc.
  void                 AddShape(Shape* aShape);
  void                 AddJoint(Joint* aJoint);
  void                 AddForce(Vector3 const &aForce);
  void                 ClearForces();
  void                 AddIgnore(HashString const &aObjectName);
  void                 RemoveIgnore(HashString const &aObjectName);
  bool                 IgnoreObject(GameObject const *aObject) const;

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
  
  bool                 IsPaused() const;
  void                 SetPaused(bool const aPaused);

  Vector3              GetBroadSize() const;
  void                 SetBroadSize(Vector3 const &aSize);
  
  IgnoreContainer      GetIgnoreList() const;
  void                 SetIgnoreList(IgnoreContainer const &aIgnoreList);
  
  ShapeContainer&      GetShapes();
  JointContainer&      GetJoints();
};

#endif /* defined(__JFramework__PhysicsObject__) */
