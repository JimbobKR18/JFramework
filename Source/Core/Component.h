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

class Component
{
private:
  GameObject* mOwner;
  std::string mName;

public:
  Component() {assert(!"Not supported.");}
  Component(std::string const &aName) : mOwner(nullptr), mName(aName) {}
  Component(Component const &aComponent) : mOwner(aComponent.mOwner) {}
  virtual ~Component() {}

  GameObject*         GetOwner() {return mOwner;}
  void                SetOwner(GameObject *aOwner) {OWNER_CHECK(); mOwner = aOwner;}
  std::string         GetDefinedName() {return mName;}

  virtual void        Update() = 0;
  virtual void        SendMessage(Message const &aMessage) = 0;
  virtual void        ReceiveMessage(Message const &aMessage) = 0;
  virtual void        Serialize(Parser &aParser) = 0;
  virtual void        Deserialize(Parser &aParser) = 0;
  static std::string  GetName() {return "Component";}
};

#endif /* defined(__JFramework__Component__) */
