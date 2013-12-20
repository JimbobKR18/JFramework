/*
 * ObjectDeleteMessage.h
 *
 *  Created on: Dec 19, 2013
 *      Author: jimmy
 */

#ifndef OBJECTDELETEMESSAGE_H_
#define OBJECTDELETEMESSAGE_H_

#include "Message.h"
#include "GameObject.h"

class ObjectDeleteMessage : public Message
{
private:
  ObjectDeleteMessage() : Message() {}
  ObjectDeleteMessage(ObjectDeleteMessage const &aMessage) : Message() {}
public:
  GameObject *mObject;
  ObjectDeleteMessage(GameObject *aObject) : mObject(aObject)
  {
    SetDescription("ObjectDelete");
  }
  virtual ~ObjectDeleteMessage() {}
};


#endif /* OBJECTDELETEMESSAGE_H_ */
