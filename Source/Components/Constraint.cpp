#include "Constraint.h"

HashType const Constraint::sUID = Common::StringHashFunction("Constraint");

// PROTECTED
Constraint::Constraint(HashType const &aUID) : Component(aUID)
{
  mJoints[0] = nullptr;
  mJoints[1] = nullptr;
}

// PUBLIC
Constraint::Constraint() : Component(Constraint::sUID)
{
  assert(!"Default Constraint constructor called, are you sure you meant to do this?");
}

Constraint::~Constraint()
{
}

/**
 * @brief Get joint.
 * @param aId Id of joint.
 * @return Joint.
 */
Joint *Constraint::GetJoint(int aId)
{
  return mJoints[aId];
}
