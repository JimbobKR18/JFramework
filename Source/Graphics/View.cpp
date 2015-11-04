#include "View.h"
#include "GameObject.h"
#include "Transform.h"

View::View(float aRate) : mTransform(), mHalfSize(0,0,0), mMaxBoundary(0,0,0),
                          mMinBoundary(0,0,0), mTarget(NULL), mRate(aRate)
{
}

View::~View()
{
}

/**
 * @brief Get position of view
 * @return position
 */
Vector3& View::GetPosition()
{
  return mTransform.GetPosition();
}

/**
 * @brief Get size of view
 * @return size
 */
Vector3& View::GetSize()
{
  return mTransform.GetSize();
}

/**
 * @brief Get rotation of view
 * @return rotation
 */
Matrix33& View::GetRotation()
{
  return mTransform.GetRotation();
}

/**
 * @brief Get scale of view
 * @return scale
 */
Vector3& View::GetScale()
{
  return mTransform.GetScale();
}

/**
 * @brief Get half size of view
 * @return half size
 */
Vector3 View::GetHalfSize() const
{
  return mHalfSize;
}

/**
 * @brief Get maximum boundary of view
 * @return max boundary
 */
Vector3 View::GetMaxBoundary() const
{
  return mMaxBoundary;
}

/**
 * @brief Get minimum boundary of view
 * @return min boundary
 */
Vector3 View::GetMinBoundary() const
{
  return mMinBoundary;
}

/**
 * @brief Get target for view
 * @return Target
 */
GameObject* View::GetTarget()
{
  return mTarget;
}

/**
 * @brief Set size of view
 * @param aSize size
 */
void View::SetSize(Vector3 const &aSize)
{
  mTransform.SetSize(aSize);
  mHalfSize = aSize / 2.0f;
}

/**
 * @brief Set position of view
 * @param aPos position
 */
void View::SetPosition(Vector3 const &aPos)
{
  mTransform.SetPosition(aPos);
}

/**
 * @brief Set view target (who to follow)
 * @param aObj target
 */
void View::SetTarget(GameObject *aObj)
{
  mTarget = aObj;
}

/**
 * @brief Set rate of motion
 * @param aRate rate
 */
void View::SetRate(float aRate)
{
  mRate = aRate;
}

/**
 * @brief Set max boundary of view
 * @param aMaxBoundary max boundary
 */
void View::SetMaxBoundary(Vector3 const &aMaxBoundary)
{
  mMaxBoundary = aMaxBoundary;
}

/**
 * @brief Set min boundary of view
 * @param aMinBoundary min boundary
 */
void View::SetMinBoundary(Vector3 const &aMinBoundary)
{
  mMinBoundary = aMinBoundary;
}

/**
 * @brief Update loop
 */
void View::Update()
{
  // Temporary position
  Vector3 &position = mTransform.GetPosition();
  
  // Follow target
  if(mTarget)
    position = mTarget->GET<Transform>()->GetPosition();

  // Boundary check
  if(position.x - mHalfSize.x < mMinBoundary.x)
    position.x = mMinBoundary.x + mHalfSize.x;
  else if(position.x + mHalfSize.x > mMaxBoundary.x)
    position.x = mMaxBoundary.x - mHalfSize.x;

  if(position.y - mHalfSize.y < mMinBoundary.y)
    position.y = mMinBoundary.y + mHalfSize.y;
  else if(position.y + mHalfSize.y > mMaxBoundary.y)
    position.y = mMaxBoundary.y - mHalfSize.y;
}
