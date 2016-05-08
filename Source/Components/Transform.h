#ifndef __JFramework_Transform_h_
#define __JFramework_Transform_h_

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

class Transform : public Component
{
private:
  Vector3     mPosition;
  Vector3     mScale;
  Vector3     mSize;
  Matrix33    mRotation;
  X_ALIGNMENT mXAlign;
  Y_ALIGNMENT mYAlign;
  Z_ALIGNMENT mZAlign;
  AxisLock    mLockedAxes;
  
  static int const sUID;

public:
  Transform();
  virtual ~Transform();

  Vector3&            GetPosition();
  void                SetPosition(Vector3 const &aPos);

  Vector3&            GetScale();
  void                SetScale(Vector3 const &aScale);

  Vector3&            GetSize();
  void                SetSize(Vector3 const &aSize);
  
  Matrix33&           GetRotation();
  void                SetRotation(Matrix33 const &aRotation);

  X_ALIGNMENT         GetXAlignment() const;
  void                SetXAlignment(X_ALIGNMENT const &aAlign);

  Y_ALIGNMENT         GetYAlignment() const;
  void                SetYAlignment(Y_ALIGNMENT const &aAlign);

  Z_ALIGNMENT         GetZAlignment() const;
  void                SetZAlignment(Z_ALIGNMENT const &aAlign);
  
  AxisLock            GetLockedAxes() const;
  void                SetLockedAxis(AxisLock const &aLockedAxes);

  void                Update() {}
  void                SendMessage(Message const &aMessage) {}
  void                ReceiveMessage(Message const &aMessage) {}
  void                Serialize(Parser &aParser);
  void                Deserialize(Parser &aParser);
  static int          GetUID() {return sUID;}
  static void         SerializeLUA();
};

#endif
