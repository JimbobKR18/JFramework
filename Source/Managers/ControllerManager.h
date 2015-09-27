//
//  ControllerManager.h
//  JFramework
//
//  Created by Jimmy Spencer on 12/26/12.
//
//

#ifndef __JFramework__ControllerManager__
#define __JFramework__ControllerManager__

#include "Manager.h"
#include "Controller.h"

class ControllerManager : public Manager
{
private:
  std::vector<Controller*> mControllers;
  
  static unsigned const sUID;
public:
  ControllerManager(GameApp* aApp);
  ~ControllerManager();
  
  void                AddController(Controller *aController);
  void                RemoveController(Controller *aController);
  void                DeleteController(Controller *aController);
  void                SetControllersPauseState(bool const aPaused);
  void                ClearControllers();
  
  // Derived from Manager
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "ControllerManager";}
  static unsigned     GetUID() {return sUID;}
  
public:
  typedef std::vector<Controller*>::iterator ControllerIT;
};

#endif /* defined(__JFramework__ControllerManager__) */
