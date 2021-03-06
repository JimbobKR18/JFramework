#include "EffectsManager.h"
#include "DefaultEffectsFactory.h"

HashType const EffectsManager::sUID = Common::StringHashFunction("EffectsManager");

EffectsManager::EffectsManager(GameApp *aApp) : Manager(aApp, "EffectsManager", EffectsManager::sUID),
  mEffects(), mEffectsFactory(new DefaultEffectsFactory())
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
  delete mEffectsFactory;
}

/**
 * @brief Set effects factory
 * @param aEffectsFactory Factory
 */
void EffectsManager::SetEffectsFactory(EffectsFactory *aEffectsFactory)
{
  if(mEffectsFactory)
    delete mEffectsFactory;
  mEffectsFactory = aEffectsFactory;
}

/**
 * @brief Create new effect using effect factory.
 * @param aType Type of effect.
 * @return New effect.
 */
Effect* EffectsManager::CreateEffect(HashString const &aType)
{
  Effect* effect = mEffectsFactory->CreateEffect(this, aType);
  AddEffect(effect);
  return effect;
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
 * @param aEffect Effect to delete
 */
void EffectsManager::RemoveEffect(Effect* aEffect)
{
  EffectIT end = mEffects.end();
  for(EffectIT it = mEffects.begin(); it != end; ++it)
  {
    if(*it == aEffect)
    {
      (*it)->Complete();
      delete *it;
      it = mEffects.erase(it);
      break;
    }
  }
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
  for(EffectIT it = mEffects.begin(); it != mEffects.end();)
  {
    if((*it)->GetName() == aName)
    {
      Effect *effect = *it;
      it = mEffects.erase(it);
      effect->Complete();
      delete effect;
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
  for(EffectIT it = mEffects.begin(); it != mEffects.end();)
  {
    if((*it)->GetObject() == aObject)
    {
      Effect *effect = *it;
      it = mEffects.erase(it);
      effect->Complete();
      delete effect;
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
 * @param aName Name to remove
 */
void EffectsManager::RemoveEffectsForObject(GameObject *aObject, HashString const &aName)
{
  for(EffectIT it = mEffects.begin(); it != mEffects.end();)
  {
    if((*it)->GetObject() == aObject && (*it)->GetName() == aName)
    {
      Effect *effect = *it;
      it = mEffects.erase(it);
      effect->Complete();
      delete effect;
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
 * @brief Query if object has effects on it
 * @param aObject Object to check
 * @return True if effect is found
 */
bool EffectsManager::ObjectHasEffect(GameObject *aObject) const
{
  if(aObject == nullptr)
    return false;
    
  for(ConstEffectIT it = mEffects.begin(); it != mEffects.end(); ++it)
  {
    if((*it)->GetObject() == aObject)
    {
      return true;
    }
  }
  return false;
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
 */
void EffectsManager::Update()
{
  EffectIT end = mEffects.end();
  for(EffectIT it = mEffects.begin(); it != end; ++it)
  {
    (*it)->Update(GetOwningApp()->GetAppStep());
  }
  for(EffectIT it = mEffects.begin(); it != mEffects.end();)
  {
    if((*it)->IsComplete())
    {
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
 * @brief Does nothing.
 * @param aMessage Message to do nothing with.
 */
void EffectsManager::ProcessDelayedMessage(Message* aMessage)
{
}

/**
 * @brief Send message to all objects.
 * @param aMessage Message to send.
 */
void EffectsManager::SendMessage(Message const& aMessage)
{
}
