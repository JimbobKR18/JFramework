#ifndef __JFramework_DefaultGameObjectFactory_H_
#define __JFramework_DefaultGameObjectFactory_H_

#include "GameObjectFactory.h"

class DefaultGameObjectFactory : public GameObjectFactory
{
public:
  DefaultGameObjectFactory();
  virtual ~DefaultGameObjectFactory();

  GameObject* CreateGameObject(ObjectManager* aManager, HashString const& aFileName, HashString const& aType);
};

#endif
