//
//  Component.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/1/12.
//
//

#ifndef __JFramework__Component__
#define __JFramework__Component__

#include "Message"

class Component
{
private:
public:
  Component();
  Component(Component const &aComponent);
  
  virtual void Update() = 0;
  virtual void SendMessage(Message const &aMessage) = 0;
  virtual void ReceiveMessage(Message const &aMessage) = 0;
};

#endif /* defined(__JFramework__Component__) */
