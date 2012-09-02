//
//  Message.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/1/12.
//
//

#ifndef JFramework_Message_h
#define JFramework_Message_h

#include <string>

// This class should be derived from, not directly instantiated;
class Message
{
private:
  std::string mDesc;
  
  Message() {}
protected:
  Message(Message const &aMessage) : mDesc(aMessage.mDesc) {}
};

#endif
