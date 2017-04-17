#include "DefaultGameObjectFactory.h"
#include "MenuButton.h"
#include "MenuText.h"
#include "ObjectManager.h"

DefaultGameObjectFactory::DefaultGameObjectFactory()
{
}

DefaultGameObjectFactory::~DefaultGameObjectFactory()
{
}

GameObject* DefaultGameObjectFactory::CreateGameObject(ObjectManager* aManager, HashString const& aFileName, HashString const& aType)
{
  return new GameObject(aManager, aFileName);
}
