//
//  PlayerController.h
//  JFramework
//
//  Created by Jimmy Spencer on 12/25/12.
//
//  The idea: Controls a player object
//
//

#ifndef __JFramework__PlayerController__
#define __JFramework__PlayerController__

#include "Controller.h"
#include "GameObject.h"

class PlayerController : public Controller
{
private:
  GameObject* mObj;
  
public:
  PlayerController();
  ~PlayerController();
  
  void SetTarget(GameObject *aTarget);
  
  // Virtuals derived from Controller
  virtual void DoAction(std::string const &aAction);
  
  // Virtuals derived from Component
  virtual void Update();
  virtual void SendMessage(Message const &aMessage);
  virtual void ReceiveMessage(Message const &aMessage);
  static std::string GetName() {return "PlayerController";}
};

#endif /* defined(__JFramework__PlayerController__) */
