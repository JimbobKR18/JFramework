#ifndef __JFramework_Transform_h_
#define __JFramework_Transform_h_

#include "Common.h"
#include "Component.h"
#include "MathExt.h"

enum AxisLock
{
  NO_AXIS = 0,
  X_AXIS = 1,
  Y_AXIS = 2,
  Z_AXIS = 4,
  XY_AXIS = 3,
  YZ_AXIS = 6,
  XZ_AXIS = 5,
  ALL_AXES = 7
};

enum ParentInherit
{
  INHERIT_NONE = 0,
  INHERIT_POSITION = 1,
  INHERIT_ROTATION = 2,
  INHERIT_SCALE = 4,
  INHERIT_POSITION_ROTATION = 3,
  INHERIT_ROTATION_SCALE = 6,
  INHERIT_POSITION_SCALE = 5,
  INHERIT_ALL = 7
};

class Transform : public Component
{
private:
  Vector3       mPosition;
  Vector3       mScale;
  Vector3       mSize;
  Matrix33      mRotation;
  X_ALIGNMENT   mXAlign;
  Y_ALIGNMENT   mYAlign;
  Z_ALIGNMENT   mZAlign;
  AxisLock      mLockedAxes;
  ParentInherit mInheritInfo;
  
  Vector3       mHierarchicalPosition;
  Vector3       mHierarchicalScale;
  Matrix33      mHierarchicalRotation;
  
  Vector3       mMaxBoundary;
  Vector3       mMinBoundary;
  
  static HashType const sUID;

public:
  Transform();
  Transform(Transform const &aTransform);
  virtual ~Transform();

  // GETTERS
  Vector3&            GetPosition();
  Vector3&            GetScale();
  Vector3&            GetSize();
  Matrix33&           GetRotation();
  X_ALIGNMENT         GetXAlignment() const;
  Y_ALIGNMENT         GetYAlignment() const;
  Z_ALIGNMENT         GetZAlignment() const;
  AxisLock            GetLockedAxes() const;
  ParentInherit       GetParentInheritanceInfo() const;
  Vector3 const&      GetHierarchicalPosition() const;
  Vector3 const&      GetHierarchicalScale() const;
  Matrix33 const&     GetHierarchicalRotation() const;
  Vector3             GetMaxBoundary() const;
  Vector3             GetMinBoundary() const;
  Matrix33            GetFinalTransform() const;
  
  // SETTERS
  void                SetPosition(Vector3 const &aPos);
  void                SetScale(Vector3 const &aScale);
  void                SetSize(Vector3 const &aSize);
  void                SetRotation(Matrix33 const &aRotation);
  void                SetXAlignment(X_ALIGNMENT const &aAlign);
  void                SetYAlignment(Y_ALIGNMENT const &aAlign);
  void                SetZAlignment(Z_ALIGNMENT const &aAlign);
  void                SetLockedAxis(AxisLock const &aLockedAxes);
  void                SetParentInheritanceInfo(ParentInherit const &aInheritInfo);
  void                SetMaxBoundary(Vector3 const &aMaxBoundary);
  void                SetMinBoundary(Vector3 const &aMinBoundary);
  
  // VIRTUALS
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage) {}
  virtual void        ReceiveMessage(Message const &aMessage) {}
  virtual void        Serialize(ParserNode *aNode);
  virtual void        Deserialize(ParserNode *aNode);
  virtual Component*  Clone(GameObject *aNewOwner) const;
  static HashType GetUID() {return sUID;}
  static void         SerializeLUA();
  
private:
  void                CalculateHierarchy();
  void                EnforceBoundaries();
};

#endif
