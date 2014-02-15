//
//  Controller.h
//  JFramework
//
//  Created by Jimmy Spencer on 12/25/12.
//
//  The idea: The outline for code that will
//            control a GameObject
//

#ifndef __JFramework__Controller__
#define __JFramework__Controller__

#include "Common.h"
#include "Component.h"
#include "MathExt.h"

class Controller : public Component
{
protected:
  Controller(std::string const &aName);
public:
  Controller();
  virtual ~Controller();
  
  virtual void        DoAction(std::string const &aAction, Vector3 const &aLocation) = 0;
  
  // Virtuals derived from Component
  virtual void        Update() = 0;
  virtual void        SendMessage(Message const &aMessage) = 0;
  virtual void        ReceiveMessage(Message const &aMessage) = 0;
  virtual void        Serialize(Parser &aParser) = 0;
  virtual void        Deserialize(Parser &aParser) = 0;
  static std::string  GetName() {return "Controller";}
};

#endif /* defined(__JFramework__Controller__) */
