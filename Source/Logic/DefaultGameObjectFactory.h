#ifndef __JFramework_DefaultGameObjectFactory_H_
#define __JFramework_DefaultGameObjectFactory_H_

#include "GameObjectFactory.h"

class DefaultGameObjectFactory : public GameObjectFactory
{
private:
  std::unordered_set<unsigned long> mIndices;
  
public:
  DefaultGameObjectFactory();
  virtual ~DefaultGameObjectFactory();

  virtual GameObject* CreateGameObject(ObjectManager* aManager, HashString const& aFileName, HashString const& aType);
  virtual void RemoveGameObject(GameObject *aObj);
};

#endif
