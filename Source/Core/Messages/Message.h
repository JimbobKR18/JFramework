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
  std::string mDesc,
              mContent;
  
protected:
  void SetDescription(std::string const &aDesc) { mDesc = aDesc; }
  void SetContent(std::string const &aContent) { mContent = aContent; }
  
public:
  Message() {}
  Message(Message const &aMessage) : mDesc(aMessage.mDesc) {}
  virtual ~Message() {}
  
  std::string GetDescription() const { return mDesc; }
  std::string GetContent() const { return mContent; }
};

#endif
