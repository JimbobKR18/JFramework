#ifndef __JFramework_GameObjectFactory_H_
#define __JFramework_GameObjectFactory_H_

#include "Common.h"
#include "GameObject.h"

class ObjectManager;
class GameObjectFactory
{
public:
  GameObjectFactory() {}
  virtual ~GameObjectFactory() {}
  
  virtual GameObject* CreateGameObject(ObjectManager *aManager, HashString const &aFileName, HashString const &aType) = 0;
};

#endif
