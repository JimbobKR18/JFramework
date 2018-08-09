#include "State.h"
#include "LUA/LUATypes.h"

State::State() : mName()
{
}

State::State(HashString const &aName) : mName(aName), mTimeAlive(INFINITE_TIME_ALIVE)
{
}

State::State(HashString const &aName, float const &aTimeAlive) : mName(aName), mTimeAlive(aTimeAlive)
{
}

State::~State()
{
}

/**
 * @brief Get name of State.
 * @return Name.
 */
HashString State::GetName() const
{
  return mName;
}

/**
 * @brief Get time alive.
 * @return Time alive.
 */
float State::GetTimeAlive() const
{
  return mTimeAlive;
}

/**
 * @brief Set name of State.
 * @param aName Name.
 */
void State::SetName(HashString const &aName)
{
  mName = aName;
}

/**
 * @brief Set time alive.
 * @param aTimeAlive Time alive.
 */
void State::SetTimeAlive(float const &aTimeAlive)
{
  mTimeAlive = aTimeAlive;
}

/**
 * @brief Serialize to LUA.
 */
void State::SerializeLUA()
{
  SLB::Class<State>("State")
    .set("GetName", &State::GetName)
    .set("GetTimeAlive", &State::GetTimeAlive);
}
