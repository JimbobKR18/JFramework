/*
 * ControllerChangeMessage.h
 *
 *  Created on: Dec 2, 2013
 *      Author: jimmy
 */

#ifndef CONTROLLERCHANGEMESSAGE_H_
#define CONTROLLERCHANGEMESSAGE_H_

#include "Message.h"
#include "Controller.h"

class ControllerChangeMessage : public Message
{
private:
  ControllerChangeMessage() : Message() {}
  ControllerChangeMessage(ControllerChangeMessage const &aMessage) : Message() {}
public:
  Controller* mController;
  GameObject* mCurrentOwner;
  GameObject* mNewOwner;
  ControllerChangeMessage(Controller* aController, GameObject* aCurrentOwner, GameObject* aNewOwner) :
                          mController(aController), mCurrentOwner(aCurrentOwner), mNewOwner(aNewOwner)
  {
    SetDescription("ControllerChange");
  }
  virtual ~ControllerChangeMessage() {}
};


#endif /* CONTROLLERCHANGEMESSAGE_H_ */
