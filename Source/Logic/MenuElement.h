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
#include "Transform.h"
#include "PCSurface.h"
#include "ObjectManager.h"
#include "GraphicsManager.h"
#include "Message.h"

class MenuElement
{
private:
  GameObject* mObject;
  int         mLayer;
  
public:
  MenuElement(std::string const &aFilename);
  
  virtual ~MenuElement();
  
  GameObject*   GetObject();
  int           GetLayer();
  void          SetLayer(int const aLayer);
  
  virtual void  Draw() = 0;
  virtual void  SendMessage(Message const &aMessage) = 0;
  virtual void  ReceiveMessage(Message const &aMessage) = 0;
  
protected:
  // For now: a helper function
  void          ParseFile(Parser &aParser);
};

#endif /* defined(__JFramework__MenuElement__) */
