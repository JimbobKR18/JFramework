#ifndef __JFramework_DefaultGameObjectFactory_H_
#define __JFramework_DefaultGameObjectFactory_H_

#include "GameObjectFactory.h"

class DefaultGameObjectFactory : public GameObjectFactory
{
private:
  unsigned mIndex;
  
public:
  DefaultGameObjectFactory();
  virtual ~DefaultGameObjectFactory();

  virtual GameObject* CreateGameObject(ObjectManager* aManager, HashString const& aFileName, HashString const& aType);
};

#endif
