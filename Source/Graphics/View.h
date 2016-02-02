#ifndef __JFramework_Camera_h_
#define __JFramework_Camera_h_

#include "MathExt.h"
#include "Transform.h"
#include "Interpolation.h"

class GameObject;

class View
{
private:
  Transform               mTransform;
  Vector3                 mHalfSize;
  Vector3                 mMaxBoundary;
  Vector3                 mMinBoundary;
  GameObject*             mTarget;
  float                   mTime;
  Interpolation<Vector3>* mInterpolator;

public:
  View(float aTime = 0.0f);
  virtual ~View();

  // Getters
  Vector3&  GetPosition();
  Vector3&  GetSize();
  Matrix33& GetRotation();
  Vector3&  GetScale();
  Matrix33  GetFinalTransform();
  Vector3   GetHalfSize() const;
  Vector3   GetMaxBoundary() const;
  Vector3   GetMinBoundary() const;
  GameObject* GetTarget();

  // Setters
  void      SetSize(Vector3 const &aSize);
  void      SetPosition(Vector3 const &aPos);
  void      SetTarget(GameObject *aObj);
  void      SetTime(float aTime);
  void      SetMaxBoundary(Vector3 const &aMaxBoundary);
  void      SetMinBoundary(Vector3 const &aMinBoundary);
  void      Update();
};

#endif
