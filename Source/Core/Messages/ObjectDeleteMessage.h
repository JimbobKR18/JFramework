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

HashString const OBJECT_DELETE = "ObjectDelete";

class ObjectDeleteMessage : public Message
{
private:
  ObjectDeleteMessage() : Message() {}
  ObjectDeleteMessage(ObjectDeleteMessage const &aMessage) : Message() {}
public:
  GameObject *mObject;
  ObjectDeleteMessage(GameObject *aObject) : mObject(aObject)
  {
    SetDescription(OBJECT_DELETE);
  }
  virtual ~ObjectDeleteMessage() {}
};


#endif /* OBJECTDELETEMESSAGE_H_ */
