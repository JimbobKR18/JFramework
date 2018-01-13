#ifndef __JFramework_ControllerFactory_h_
#define __JFramework_ControllerFactory_h_

#include "ControllerManager.h"
#include "Controller.h"

class ControllerFactory
{
public:
  ControllerFactory() {};
  virtual ~ControllerFactory() {};
  
  /**
   * @brief Create a new controller.
   * @param aManager Manager to handle controller.
   * @param aName Name of controller type.
   * @param aData Any extra data needed to create controller.
   * @return New controller.
   */
  virtual Controller* CreateController(ControllerManager *aManager, HashString const &aName, ParserNode* const aData=nullptr) = 0;
};

#endif // __JFramework_ControllerFactory_h_
