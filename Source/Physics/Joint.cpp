#include "Joint.h"
#include "PhysicsObject.h"

Joint::Joint(int aId, Vector3 const &aPosition, PhysicsObject *aOwner) : mId(aId), mPosition(aPosition), mOwner(aOwner)
{  
}

Joint::~Joint()
{
}

/**
 * @brief Get Id.
 * @return Id.
 */
int const& Joint::GetID() const
{
  return mId;
}

/**
 * @brief Get position.
 * @return Position.
 */
Vector3 const& Joint::GetPosition() const
{
  return mPosition;
}

/**
 * @brief Get owner.
 * @return Owner.
 */
PhysicsObject *Joint::GetOwner() const
{
  return mOwner;
}

/**
 * @brief Set id.
 * @param aId Id.
 */
void Joint::SetID(int const aId)
{
  mId = aId;
}

/**
 * @brief Set position.
 * @param aPosition Position.
 */
void Joint::SetPosition(Vector3 const &aPosition)
{
  mPosition = aPosition;
}
