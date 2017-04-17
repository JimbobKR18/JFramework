//
//  Message.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/1/12.
//
//

#ifndef JFramework_Message_h
#define JFramework_Message_h

#include "HashString.h"

// This class should be derived from, not directly instantiated;
class Message
{
private:
  HashString mDesc;
  HashString mContent;
  
public:
  Message() {}
  Message(Message const &aMessage) : mDesc(aMessage.mDesc), mContent(aMessage.mContent) {}
  virtual ~Message() {}

protected:
  void        SetDescription(HashString const &aDesc) { mDesc = aDesc; }
  void        SetContent(HashString const &aContent) { mContent = aContent; }
  
public:
  HashString  GetDescription() const { return mDesc; }
  HashString  GetContent() const { return mContent; }
};

#endif
