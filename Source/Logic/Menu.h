/*
 * Menu.h
 *
 *  Created on: Feb 3, 2013
 *      Author: jimbob
 */

#ifndef __JFramework_Menu_h_
#define __JFramework_Menu_h_

#include "MenuElement.h"
#include "Level.h"

class Menu
{
private:
  Level*                         mOwner;
  HashString                     mName;
  HashString                     mFileName;
  MenuElement::ElementContainer  mMenuElements;
  MenuElement::ElementContainer  mReplaceableElements;

public:
  Menu(Level* aLevel, HashString const &aFileName);
  virtual ~Menu();

  HashString                     GetName() const;
  HashString                     GetFileName() const;
  Level*                         GetLevel() const;
  MenuElement*                   GetElement(HashString const &aName) const;
  MenuElement*                   GetElementByObjectName(HashString const &aObjectName) const;
  MenuElement*                   GetElementByFileName(HashString const &aFileName) const;
  MenuElement::ElementContainer  GetElements() const;

  void                           AddObject(MenuElement *aElement);
  void                           AddReplaceableObject(MenuElement *aElement);
  void                           DeleteObject(MenuElement *aElement);
  void                           DeleteObjects();
  void                           Update();
  
  virtual void                   ReceiveMessage(Message const& aMessage);
  virtual void                   SendMessage(Message const& aMessage);
  
private:
  void                           ParseFile();
  void                           ParseTransform(GameObject *aObject, Root *aTransform);
  void                           ParseSurface(GameObject *aObject, Root *aSurface);
  void                           ParseEffects(GameObject *aObject, Root *aEffects);
  void                           DeleteElementChildren(MenuElement *aElement, bool aReplaceable);
};

#endif /* MENU_H_ */
