#ifndef __JFramework_Camera_h_
#define __JFramework_Camera_h_

#include "MathExt.h"

class GameObject;

class View
{
private:
	Vector3     mPosition;
  Vector3     mSize;
  Vector3     mHalfSize;
  Vector3     mMaxBoundary;
  Vector3     mMinBoundary;
	GameObject* mTarget;
	float       mRate;

public:
	View(float aRate = 0.0f);
	~View();

	// Getters
	Vector3   GetPosition() const;
  Vector3   GetSize() const;
  Vector3   GetMaxBoundary() const;
  Vector3   GetMinBoundary() const;

  // Setters
  void      SetSize(Vector3 const &aSize);
	void      SetPosition(Vector3 const &aPos);
	void      SetTarget(GameObject *aObj);
	void      SetRate(float aRate);
	void      SetMaxBoundary(Vector3 const &aMaxBoundary);
	void      SetMinBoundary(Vector3 const &aMinBoundary);
	void      Update();
};

#endif
