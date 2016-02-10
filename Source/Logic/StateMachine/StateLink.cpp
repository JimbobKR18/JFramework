#include "StateLink.h"

StateLink::StateLink() : mStart(nullptr), mEnd(nullptr)
{
}

StateLink::StateLink(State *aStart, State *aEnd) : mStart(aStart), mEnd(aEnd)
{
}

StateLink::~StateLink()
{
}

/**
 * @brief Get start of StateLink
 * @return Start
 */
State *StateLink::GetStart() const
{
  return mStart;
}

/**
 * @brief Get end of StateLink
 * @return End
 */
State *StateLink::GetEnd() const
{
  return mEnd;
}