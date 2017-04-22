#include "DefaultEffectsFactory.h"

DefaultEffectsFactory::DefaultEffectsFactory()
{
}

DefaultEffectsFactory::~DefaultEffectsFactory()
{
}

/**
 * @brief Create new effect.
 * @param aManager Manager associated with creation.
 * @param aName Name of effect.
 * @return New effect.
 */
Effect* DefaultEffectsFactory::CreateEffect(EffectsManager* aManager, HashString const& aName)
{
  assert(!"Do not use this method, rather, create your own factory derived from this.");
  return nullptr;
}
