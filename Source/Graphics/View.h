#ifndef __JFramework_Camera_h_
#define __JFramework_Camera_h_

#include "MathExt.h"

class GameObject;

class View
{
private:
	Vector3 mPosition;
  Vector3 mSize;
	GameObject *mTarget;
	float mRate;

public:
	View(float aRate = 0.0f);
	~View();

	Vector3 GetPosition() const;
  Vector3 GetSize() const;

  void SetSize(Vector3 const &aSize);
	void SetPosition(Vector3 const &aPos);
	void SetTarget(GameObject *aObj);
	void SetRate(float aRate);

	void Update();
};

#endif
