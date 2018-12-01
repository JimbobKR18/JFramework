#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include "Common.h"
#include "Component.h"
#include "Parser.h"

class GameApp;
class ComponentFactory
{
public:
  ComponentFactory() {}
  virtual ~ComponentFactory() {}
  
  /**
   * @brief Create component, add to object, and deserialize.
   * @param aApp Game app.
   * @param aObject Object to add component to.
   * @param aNode Node that contains component info.
   * @return New component.
   */
  virtual Component* CreateComponent(GameApp* aApp, GameObject *aObject, ParserNode *aNode) = 0;
  
  /**
   * @brief Delete a component.
   * @param aApp Game app.
   * @param aObject Object to delete component from.
   * @param aComponent
   */
  virtual void DeleteComponent(GameApp* aApp, GameObject *aObject, Component *aComponent) = 0;
};

#endif // COMPONENTFACTORY_H
