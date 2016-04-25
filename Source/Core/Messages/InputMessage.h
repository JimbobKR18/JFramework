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
  int mId;

  InputMessage() : Message(), mLocation(0,0,0), mId(0) {}
  InputMessage(InputMessage const &aMessage) : Message(), mLocation(aMessage.mLocation), mId(aMessage.mId) {}
  
public:
  InputMessage(HashString const &aContent, Vector3 const &aLocation, int const aId = 0) : Message(), mLocation(aLocation), mId(aId)
  {
    SetDescription("Input");
    SetContent(aContent);
  }
  virtual ~InputMessage() {}

  Vector3 GetLocation() const {return mLocation;}
  int GetId() const {return mId;}
};

#endif /* defined(__JFramework__InputMessage__) */
