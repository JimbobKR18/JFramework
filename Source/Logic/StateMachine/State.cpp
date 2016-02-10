#include "State.h"

State::State() : mName()
{
}

State::State(HashString const &aName) : mName(aName)
{
}

State::~State()
{
}

/**
 * @brief Get name of State.
 * @return Name.
 */
HashString const State::GetName() const
{
  return mName;
}

/**
 * @brief Set name of State.
 * @param aName Name.
 */
void State::SetName(HashString const &aName)
{
  mName = aName;
}