//
//  InputMessage.h
//  JFramework
//
//  Created by Jimmy Spencer on 12/27/12.
//  The idea: Message to handle input
//
//

#ifndef __JFramework__InputMessage__
#define __JFramework__InputMessage__

#include "Message.h"

class InputMessage : public Message
{
private:
  InputMessage() : Message() {}
  InputMessage(InputMessage const &aMessage) : Message() {}
  
public:
  InputMessage(std::string const &aContent) : Message()
  {
    SetDescription("Input");
    SetContent(aContent);
  }
  ~InputMessage() {}
};

#endif /* defined(__JFramework__InputMessage__) */
