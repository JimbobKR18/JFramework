#include "DefaultGameObjectFactory.h"
#include "MenuButton.h"
#include "MenuText.h"
#include "ObjectManager.h"

DefaultGameObjectFactory::DefaultGameObjectFactory() : mIndex(0)
{
}

DefaultGameObjectFactory::~DefaultGameObjectFactory()
{
}

/**
 * @brief Create new game object.
 * @param aManager Manager that owns game object.
 * @param aFileName File for new object.
 * @param aType Type of object.
 * @return New object.
 */
GameObject* DefaultGameObjectFactory::CreateGameObject(ObjectManager* aManager, HashString const& aFileName, HashString const& aType)
{
  return new GameObject(aManager, mIndex++, aFileName);
}
