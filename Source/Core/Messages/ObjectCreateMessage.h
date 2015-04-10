/*
 * ObjectCreateMessage.h
 *
 *  Created on: Apr 10, 2015
 *      Author: jimmy
 */

#ifndef OBJECTCREATEMESSAGE_H_
#define OBJECTCREATEMESSAGE_H_

#include "Message.h"
#include "GameObject.h"

HashString const OBJECT_CREATE = "ObjectCreate";

class ObjectCreateMessage : public Message
{
private:
  ObjectCreateMessage() : Message() {}
  ObjectCreateMessage(ObjectCreateMessage const &aMessage) : Message() {}
public:
  GameObject *mObject;
  ObjectCreateMessage(GameObject *aObject) : mObject(aObject)
  {
    SetDescription(OBJECT_CREATE);
  }
  virtual ~ObjectCreateMessage() {}
};


#endif /* OBJECTDELETEMESSAGE_H_ */
