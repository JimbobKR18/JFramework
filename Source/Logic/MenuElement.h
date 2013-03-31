//
//  MenuElement.h
//  JFramework
//
//  Created by Jimmy Spencer on 3/25/13.
//
//

#ifndef __JFramework__MenuElement__
#define __JFramework__MenuElement__

#include "GameObject.h"
#include "TextParser.h"
#include "Message.h"

class MenuElement
{
private:
  GameObject* mObject;
  int         mLayer;
  
public:
  MenuElement(std::string const &aFilename)
  {
    mObject = new GameObject(nullptr, aFilename);
    mLayer = 0;
  }
  
  ~MenuElement() { delete mObject; }
  
  int           GetLayer() { return mLayer; }
  void          SetLayer(int const aLayer) { mLayer = aLayer; }
  
  virtual void  Draw() = 0;
  virtual void  SendMessage(Message const &aMessage) = 0;
  virtual void  ReceiveMessage(Message const &aMessage) = 0;
};

#endif /* defined(__JFramework__MenuElement__) */
