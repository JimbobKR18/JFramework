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
#include "MathExt.h"

class InputMessage : public Message
{
private:
  Vector3 mLocation;

  InputMessage() : Message(), mLocation(0,0,0) {}
  InputMessage(InputMessage const &aMessage) : Message(), mLocation(aMessage.mLocation) {}
  
public:
  InputMessage(std::string const &aContent, Vector3 const &aLocation) : Message(), mLocation(aLocation)
  {
    SetDescription("Input");
    SetContent(aContent);
  }
  ~InputMessage() {}

  Vector3 GetLocation() const {return mLocation;}
};

#endif /* defined(__JFramework__InputMessage__) */
