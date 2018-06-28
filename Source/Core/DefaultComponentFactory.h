#ifndef DEFAULTCOMPONENTFACTORY_H
#define DEFAULTCOMPONENTFACTORY_H

#include "ComponentFactory.h"

class DefaultComponentFactory : public ComponentFactory
{
public:
  DefaultComponentFactory();
  virtual ~DefaultComponentFactory();

  virtual Component* CreateComponent(GameApp* aApp, GameObject *aObject, ParserNode *aNode);
};

#endif // DEFAULTCOMPONENTFACTORY_H
