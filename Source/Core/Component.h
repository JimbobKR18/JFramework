//
//  Component.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/1/12.
//
//

#ifndef __JFramework__Component__
#define __JFramework__Component__

#include "GameObject.h"
#include "Message.h"
#include "Parser.h"
#include <assert.h>

#define OWNER_CHECK() if(mOwner && aOwner != nullptr) assert(!"This component already has an owner.")
#define DEFAULT_COMPONENT_UID 0

class Component
{
private:
  GameObject* mOwner;
  int mUID;

public:
  Component() {assert(!"Not supported.");}
  Component(int const &aUID) : mOwner(nullptr), mUID(aUID) {}
  Component(Component const &aComponent) : mOwner(aComponent.mOwner) {}
  virtual ~Component() {}

  GameObject*         GetOwner() {return mOwner;}
  void                SetOwner(GameObject *aOwner) {OWNER_CHECK(); mOwner = aOwner;}
  int                  GetDefinedUID() {return mUID;}

  virtual void        Update() = 0;
  virtual void        SendMessage(Message const &aMessage) = 0;
  virtual void        ReceiveMessage(Message const &aMessage) = 0;
  virtual void        Serialize(Parser &aParser) = 0;
  virtual void        Deserialize(Parser &aParser) = 0;
  static int           GetUID() {return DEFAULT_COMPONENT_UID;}
};

#endif /* defined(__JFramework__Component__) */
