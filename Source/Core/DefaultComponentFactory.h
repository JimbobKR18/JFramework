#ifndef DEFAULTCOMPONENTFACTORY_H
#define DEFAULTCOMPONENTFACTORY_H

#include "ComponentFactory.h"

class DefaultComponentFactory : public ComponentFactory
{
public:
  DefaultComponentFactory();
  virtual ~DefaultComponentFactory();

  virtual Component* CreateComponent(GameApp* aApp, GameObject *aObject, ParserNode *aNode);
  virtual void DeleteComponent(GameApp* aApp, GameObject *aObject, Component *aComponent);
};

#endif // DEFAULTCOMPONENTFACTORY_H
