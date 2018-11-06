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

class ControllerManager;

class Controller : public Component
{
private:
  ControllerManager *mManager;
  static int const sUID;
  bool mPaused;
protected:
  Controller(ControllerManager *aManager, int const &aUID);
public:
  Controller();
  virtual ~Controller();
  
  ControllerManager*    GetManager() const;
  void                  SetPaused(bool const aPaused);
  bool                  GetPaused() const;
  
  virtual void        DoAction(HashString const &aAction, Vector3 const &aLocation) = 0;
  
  // Virtuals derived from Component
  virtual void        Update() = 0;
  virtual void        SendMessage(Message const &aMessage) = 0;
  virtual void        ReceiveMessage(Message const &aMessage) = 0;
  virtual void        Serialize(ParserNode *aNode) = 0;
  virtual void        Deserialize(ParserNode *aNode) = 0;
  virtual Component*  Clone(GameObject *aNewOwner) const = 0;
  static int          GetUID() {return sUID;}
};

#endif /* defined(__JFramework__Controller__) */
