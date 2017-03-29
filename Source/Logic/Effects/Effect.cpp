#include "Effect.h"

Effect::Effect(HashString const &aName, GameObject *aObject, float aTime) : mObject(aObject), mTime(aTime), mName(aName) 
{
}

Effect::~Effect() 
{
}

/**
 * @brief Get target object
 * @return Object
 */
GameObject* Effect::GetObject() const
{
  return mObject;
}

/**
 * @brief Get time alive
 * @return Time alive
 */
float const Effect::GetTime() const
{
  return mTime;
}

/**
 * @brief Get name
 * @return Name
 */
HashString const Effect::GetName() const
{
  return mName;
}
