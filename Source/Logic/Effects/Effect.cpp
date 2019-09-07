#include "Effect.h"

Effect::Effect() : mObject(nullptr), mTime(0), mName("") 
{
}

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
HashString const& Effect::GetName() const
{
  return mName;
}

/**
 * @brief Set target object
 * @param aObject Object
 */
void Effect::SetObject(GameObject* aObject)
{
  mObject = aObject;
}

/**
 * @brief Set time alive
 * @param aTime Time alive
 */
void Effect::SetTime(float aTime)
{
  mTime = aTime;
}

/**
 * @brief Set name
 * @param aName Name
 */
void Effect::SetName(HashString const &aName)
{
  mName = aName;
}
