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
#include "Shape.h"
#include "GameObject.h"
#include "Core/LuaIncludes.h"

HashString const COLLISION = "Collision";

class CollisionMessage: public Message
{
private:
  GameObject*   mObject1;
  GameObject*   mObject2;
  Shape*        mShape1;
  Shape*        mShape2;
  float         mPenetration;
  Vector3       mNormal;
  Vector3       mRelativeVelocity;
  Vector3       mContactPoint;

public:
  CollisionMessage() : Message()
  {
    assert(!"Do not create collision messages this way.");
  }
  CollisionMessage(CollisionMessage const &aMessage) : Message()
  {
    assert(!"Do not create collision messages this way.");
  }
  CollisionMessage(HashString const &aContent,
                   GameObject *aObject1,
                   GameObject *aObject2,
                   Shape *aShape1,
                   Shape *aShape2,
                   float const aPenetration,
                   Vector3 const &aNormal,
                   Vector3 const &aRelativeVelocity,
                   Vector3 const &aContactPoint) : Message(),
                                     mObject1(aObject1),
                                     mObject2(aObject2),
                                     mShape1(aShape1),
                                     mShape2(aShape2),
                                     mPenetration(aPenetration),
                                     mNormal(aNormal),
                                     mRelativeVelocity(aRelativeVelocity),
                                     mContactPoint(aContactPoint)
  {
    SetDescription(COLLISION);
    SetContent(aContent);
  }
  virtual ~CollisionMessage()
  {
  }

  GameObject *GetObject(int index) const
  {
    // Stay in bounds
    assert(index < 2);

    if(index)
      return mObject1;
    else
      return mObject2;
  }
  
  Shape *GetShape(int index) const
  {
    // Stay in bounds
    assert(index < 2);

    if(index)
      return mShape1;
    else
      return mShape2;
  }
  
  float const GetPenetration() const
  {
    return mPenetration;
  }
  
  Vector3 const &GetNormal() const
  {
    return mNormal;
  }
  
  Vector3 const &GetRelativeVelocity() const
  {
    return mRelativeVelocity;
  }
  
  Vector3 const &GetContactPoint() const
  {
    return mContactPoint;
  }
  
  static void SerializeLUA() 
  {
    SLB::Class<CollisionMessage>("CollisionMessage")
      .set("GetObject", &CollisionMessage::GetObject)
      .set("GetShape", &CollisionMessage::GetShape)
      .set("GetPenetration", &CollisionMessage::GetPenetration)
      .set("GetNormal", &CollisionMessage::GetNormal)
      .set("GetRelativeVelocity", &CollisionMessage::GetRelativeVelocity)
      .set("GetContactPoint", &CollisionMessage::GetContactPoint);
  }
};

#endif /* defined(__JFramework__CollisionMessage__) */
