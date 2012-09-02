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
#include <assert.h>


class Component
{
private:
  std::string mName;
  GameObject *mOwner;
  
public:
  Component() {assert(0);}
  Component(std::string const &aName) : mName(aName) {}
  Component(Component const &aComponent) : mName(aComponent.mName) {}
  virtual ~Component();
  
  GameObject *GetOwner() {return mOwner;}
  std::string GetName() {return mName;}
  
  virtual void Update() = 0;
  virtual void SendMessage(Message const &aMessage) = 0;
  virtual void ReceiveMessage(Message const &aMessage) = 0;
};

#endif /* defined(__JFramework__Component__) */
