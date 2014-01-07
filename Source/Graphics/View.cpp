#include "View.h"
#include "GameObject.h"
#include "Transform.h"

View::View(float aRate) : mPosition(0,0,0), mSize(0,0,0), mHalfSize(0,0,0), mMaxBoundary(0,0,0),
                          mMinBoundary(0,0,0), mTarget(NULL), mRate(aRate)
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
Vector3 View::GetMaxBoundary() const
{
  return mMaxBoundary;
}
Vector3 View::GetMinBoundary() const
{
  return mMinBoundary;
}

void View::SetSize(Vector3 const &aSize)
{
  mSize = aSize;
  mHalfSize = mSize / 2.0f;
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
void View::SetMaxBoundary(Vector3 const &aMaxBoundary)
{
  mMaxBoundary = aMaxBoundary;
}
void View::SetMinBoundary(Vector3 const &aMinBoundary)
{
  mMinBoundary = aMinBoundary;
}

void View::Update()
{
  // Follow target
	if(mTarget)
		mPosition = mTarget->GET<Transform>()->GetPosition();

	// Boundary check
	if(mPosition.x - mHalfSize.x < mMinBoundary.x)
	  mPosition.x = mMinBoundary.x + mHalfSize.x;
	else if(mPosition.x + mHalfSize.x > mMaxBoundary.x)
	  mPosition.x = mMaxBoundary.x - mHalfSize.x;

	if(mPosition.y - mHalfSize.y < mMinBoundary.y)
    mPosition.y = mMinBoundary.y + mHalfSize.y;
  else if(mPosition.y + mHalfSize.y > mMaxBoundary.y)
    mPosition.y = mMaxBoundary.y - mHalfSize.y;
}
