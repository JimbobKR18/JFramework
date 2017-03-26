#include "StateLink.h"

StateLink::StateLink() : mStart(nullptr), mEnd(nullptr), mType(BIDIRECTIONAL)
{
}

StateLink::StateLink(State *aStart, State *aEnd, LinkType const &aType) : mStart(aStart), mEnd(aEnd), mType(aType)
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

/**
 * @brief Get type of link.
 * @return Link type.
 */
StateLink::LinkType StateLink::GetType() const
{
  return mType;
}