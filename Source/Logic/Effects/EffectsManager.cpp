#include "EffectsManager.h"

EffectsManager::EffectsManager()
{
}

EffectsManager::~EffectsManager()
{
  EffectIT end = mEffects.end();
  for(EffectIT it = mEffects.begin(); it != end; ++it)
  {
    delete *it;
  }
  mEffects.clear();
}

/**
 * @brief Add Effect
 * @param aEffect Effect to add
 */
void EffectsManager::AddEffect(Effect* aEffect)
{
  mEffects.push_back(aEffect);
}

/**
 * @brief Remove effect, completes effect before deletion
 * @param aName Name of effect to delete
 */
void EffectsManager::RemoveEffect(HashString const &aName)
{
  EffectIT end = mEffects.end();
  for(EffectIT it = mEffects.begin(); it != end; ++it)
  {
    if((*it)->GetName() == aName)
    {
      (*it)->Complete();
      delete *it;
      it = mEffects.erase(it);
      break;
    }
  }
}

/**
 * @brief Remove all effects with name, completes effects before deletion
 * @param aName Name to remove
 */
void EffectsManager::RemoveMatchingEffects(HashString const &aName)
{
  EffectIT end = mEffects.end();
  for(EffectIT it = mEffects.begin(); it != end;)
  {
    if((*it)->GetName() == aName)
    {
      (*it)->Complete();
      delete *it;
      it = mEffects.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

/**
 * @brief Remove all effects with object, completes effects before deletion
 * @param aObject Object to remove
 */
void EffectsManager::RemoveEffectsForObject(GameObject *aObject)
{
  EffectIT end = mEffects.end();
  for(EffectIT it = mEffects.begin(); it != end;)
  {
    if((*it)->GetObject() == aObject)
    {
      (*it)->Complete();
      delete *it;
      it = mEffects.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

/**
 * @brief Get effect by name
 * @param aName Name of effect to get
 * @return Effect if found, nullptr otherwise
 */
Effect *EffectsManager::GetEffect(HashString const &aName)
{
  EffectIT end = mEffects.end();
  for(EffectIT it = mEffects.begin(); it != end; ++it)
  {
    if((*it)->GetName() == aName)
      return *it;
  }
  return nullptr;
}

/**
 * @brief Checks if empty
 * @return True if yes, false if no
 */
bool EffectsManager::Empty()
{
  return mEffects.empty();
}

/**
 * @brief Removes all effects, does not complete effects.
 */
void EffectsManager::RemoveEffects()
{
  EffectIT end = mEffects.end();
  for(EffectIT it = mEffects.begin(); it != end; ++it)
  {
    delete *it;
  }
  mEffects.clear();
}

/**
 * @brief Update loop
 * @param aDT Time passage, in seconds.
 */
void EffectsManager::Update(float const aDT)
{
  EffectIT end = mEffects.end();
  for(EffectIT it = mEffects.begin(); it != end; ++it)
  {
    (*it)->Update(aDT);
  }
  for(EffectIT it = mEffects.begin(); it != end;)
  {
    if((*it)->IsComplete())
    {
      delete *it;
      it = mEffects.erase(it);
      end = mEffects.end();
    }
    else
    {
      ++it;
    }
  }
}
