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
#include "GameObject.h"

HashString const COLLISION = "Collision";

class CollisionMessage: public Message
{
private:
  GameObject*   mObject1;
  GameObject*   mObject2;
  Shape*        mShape1;
  Shape*        mShape2;

  CollisionMessage() : Message()
  {
  }
  CollisionMessage(CollisionMessage const &aMessage) : Message()
  {
  }

public:
  CollisionMessage(HashString const &aContent,
                   GameObject *aObject1,
                   GameObject *aObject2,
                   Shape *aShape1,
                   Shape *aShape2) : Message(),
                                     mObject1(aObject1),
                                     mObject2(aObject2),
                                     mShape1(aShape1),
                                     mShape2(aShape2)
  {
    SetDescription(COLLISION);
    SetContent(aContent);
  }
  virtual ~CollisionMessage()
  {
  }

  GameObject *GetObject(int index)
  {
    // Stay in bounds
    assert(index < 2);

    if(index)
      return mObject1;
    else
      return mObject2;
  }
  
  Shape *GetShape(int index)
  {
    // Stay in bounds
    assert(index < 2);

    if(index)
      return mShape1;
    else
      return mShape2;
  }
};

#endif /* defined(__JFramework__CollisionMessage__) */
