#include "View.h"
#include "GameObject.h"
#include "Transform.h"

View::View(float aRate) : mPosition(0,0,0), mSize(0,0,0), mTarget(NULL), mRate(aRate)
{
}
View::~View()
{

}

Vector3 View::GetPosition() const
{
	return mPosition;
}
Vector3 View::GetSize() const
{
  return mSize;
}

void View::SetSize(Vector3 const &aSize)
{
  mSize = aSize;
}
void View::SetPosition(Vector3 const &aPos)
{
	mPosition = aPos;
}
void View::SetTarget(GameObject *aObj)
{
	mTarget = aObj;
}
void View::SetRate(float aRate)
{
	mRate = aRate;
}

void View::Update()
{
	if(mTarget)
	{
		mPosition = mTarget->GET<Transform>()->GetPosition();
	}
}
