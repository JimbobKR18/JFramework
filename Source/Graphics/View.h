#ifndef __JFramework_Camera_h_
#define __JFramework_Camera_h_

#include "Math.h"

class GameObject;

class View
{
private:
	Vector3 mPosition;
	GameObject *mTarget;
	float mRate;

public:
	View(float aRate = 0.0f);
	~View();

	Vector3 GetPosition() const;

	void SetPosition(Vector3 const &aPos);
	void SetTarget(GameObject *aObj);
	void SetRate(float aRate);

	void Update();
};

#endif
