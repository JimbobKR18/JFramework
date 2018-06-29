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

class ControllerFactory;

class ControllerManager : public Manager
{
private:
  std::set<Controller*> mControllers;
  ControllerFactory *mFactory;
  
  static unsigned const sUID;
public:
  ControllerManager(GameApp* aApp);
  ~ControllerManager();
  
  // Setters
  void                SetControllerFactory(ControllerFactory* aFactory);
  
  // Management
  Controller*         CreateController(HashString const &aName, ParserNode* const aData);
  void                DeleteController(Controller *aController);
  void                AddController(Controller *aController);
  void                RemoveController(Controller *aController);
  void                SetControllersPauseState(bool const aPaused);
  void                ClearControllers();
  
  // Derived from Manager
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "ControllerManager";}
  static unsigned     GetUID() {return sUID;}
  
  static void         SerializeLUA();
  
public:
  typedef std::set<Controller*>::iterator ControllerIT;
};

#endif /* defined(__JFramework__ControllerManager__) */
