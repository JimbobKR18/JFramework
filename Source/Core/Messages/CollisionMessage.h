//
//  CollisionMessage.h
//  JFramework
//
//  Created by Jimmy Spencer on 12/27/12.
//  The idea: Message to handle collision
//
//

#ifndef __JFramework__CollisionMessage__
#define __JFramework__CollisionMessage__

#include "Message.h"
#include "MathExt.h"

class CollisionMessage: public Message
{
private:
  CollisionMessage() : Message()
  {
  }
  CollisionMessage(CollisionMessage const &aMessage) : Message()
  {
  }

public:
  CollisionMessage(std::string const &aContent) : Message()
  {
    SetDescription("Collision");
    SetContent(aContent);
  }
  ~CollisionMessage()
  {
  }
};

#endif /* defined(__JFramework__CollisionMessage__) */
