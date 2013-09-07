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
#include "Menu.h"
#include "Common.h"

class PlayerController : public Controller
{
private:
  GameObject*             mObj;
  std::set<GameObject*>   mCollidedObjects;
  Menu*                   mGameMenu;
  
public:
  PlayerController();
  virtual ~PlayerController();
  
  void SetTarget(GameObject *aTarget);
  
  // Virtuals derived from Controller
  virtual void        DoAction(std::string const &aAction, Vector3 const &aLocation);
  
  // Virtuals derived from Component
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ReceiveMessage(Message const &aMessage);
  virtual void        Serialize(Parser &aParser);
  virtual void        Deserialize(Parser &aParser);
  static std::string  GetName() {return "PlayerController";}
};

#endif /* defined(__JFramework__PlayerController__) */
