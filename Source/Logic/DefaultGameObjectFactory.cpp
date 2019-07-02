#include "DefaultGameObjectFactory.h"
#include "ObjectManager.h"

DefaultGameObjectFactory::DefaultGameObjectFactory() : mIndices(), mCurrentIndex(0)
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
  ++mCurrentIndex;
  while(mIndices.find(mCurrentIndex) != mIndices.end())
    ++mCurrentIndex;
  mIndices.insert(mCurrentIndex);
  return new GameObject(aManager, mCurrentIndex, aFileName, aType);
}

/**
 * @brief Remove game object.
 * @param aObj Object to remove.
 */
void DefaultGameObjectFactory::RemoveGameObject(GameObject *aObj)
{
  mIndices.erase(aObj->GetID());
}
