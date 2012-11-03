#include "View.h"
#include "GameObject.h"

View::View(float aRate) : mPosition(0,0,0), mTarget(NULL), mRate(aRate)
{
}
View::~View()
{

}

Vector3 View::GetPosition() const
{
	return mPosition;
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

	}
}
