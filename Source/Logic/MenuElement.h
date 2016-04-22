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
#include "PCShaderSurface.h"
#include "ObjectManager.h"
#include "LevelManager.h"
#include "Message.h"

class MenuElement
{
protected:
  LevelManager* mManager;
  GameObject* mObject;
  int         mLayer;
  
public:
  MenuElement(HashString const &aFileName);
  
  virtual ~MenuElement();
  
  GameObject*   GetObject();
  int           GetLayer();
  void          SetLayer(int const aLayer);
  
  virtual void  Draw() = 0;
  virtual void  Update() = 0;
  virtual void  SendMessage(Message const &aMessage) = 0;
  virtual void  ReceiveMessage(Message const &aMessage) = 0;
  virtual void  ParseAdditionalData(Parser &aParser) = 0;
  
protected:
  void          ParseFile(Parser &aParser);
};

#endif /* defined(__JFramework__MenuElement__) */
