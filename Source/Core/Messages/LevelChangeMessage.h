/*
 * LevelChangeMessage.h
 *
 *  Created on: Jun 26, 2013
 *      Author: jimbob
 */

#ifndef __JFramework_LevelChangeMessage_h_
#define __JFramework_LevelChangeMessage_h_

#include "Message.h"

class LevelChangeMessage : public Message
{
private:
  LevelChangeMessage() : Message() {}
  LevelChangeMessage(LevelChangeMessage const &aMessage) : Message() {}

public:
  LevelChangeMessage(std::string const &aLevelName, std::string const &aReset) : Message()
  {
    SetDescription(aLevelName);
    SetContent(aReset);
  }
  virtual ~LevelChangeMessage() {}
};

#endif /* LEVELCHANGEMESSAGE_H_ */
