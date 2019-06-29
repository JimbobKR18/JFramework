#include "DefaultGameObjectFactory.h"
#include "ObjectManager.h"

DefaultGameObjectFactory::DefaultGameObjectFactory() : mIndices()
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
  unsigned long index = Common::lrand() % LONG_MAX;
  while(mIndices.find(index) != mIndices.end())
    index = Common::lrand() % LONG_MAX;
  mIndices.insert(index);
  return new GameObject(aManager, index, aFileName, aType);
}

/**
 * @brief Remove game object.
 * @param aObj Object to remove.
 */
void DefaultGameObjectFactory::RemoveGameObject(GameObject *aObj)
{
  mIndices.erase(aObj->GetID());
}
