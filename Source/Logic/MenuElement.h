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
#include "ObjectManager.h"
#include "LevelManager.h"
#include "Message.h"

class MenuElement
{
public:
  typedef std::set<MenuElement*>   ElementContainer;
  typedef ElementContainer::iterator  ElementIT;
  typedef ElementContainer::const_iterator  ConstElementIT;
  
protected:
  Menu*       mOwner;
  GameObject* mObject;
  bool        mReplaceable;
  
public:
  MenuElement(Menu *aOwner, HashString const &aFileName, bool const aReplaceable);
  virtual ~MenuElement();
  
  // GETTERS
  Menu*         GetOwner();
  GameObject*   GetObject();
  bool          IsReplaceable() const;
  
  // SETTERS
  void          SetOwner(Menu* aOwner);
  
  // VIRTUALS
  virtual void  Draw() = 0;
  virtual void  Update() = 0;
  virtual void  SendMessage(Message const &aMessage) = 0;
  virtual void  ReceiveMessage(Message const &aMessage) = 0;
  virtual void  ParseAdditionalData(Parser &aParser) = 0;
  
protected:
  void          ParseFile(Parser &aParser);
};

#endif /* defined(__JFramework__MenuElement__) */
