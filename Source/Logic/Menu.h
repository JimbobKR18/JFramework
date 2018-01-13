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
public:
  // For shallow removal.
  friend class Level;
  
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
  MenuElement*                   GetReplaceableElement(HashString const &aName) const;
  MenuElement*                   GetReplaceableElementByObjectName(HashString const &aObjectName) const;
  MenuElement*                   GetReplaceableElementByFileName(HashString const &aFileName) const;
  MenuElement::ElementContainer  GetReplaceableElements() const;

  void                           AddObject(MenuElement *aElement);
  void                           AddReplaceableObject(MenuElement *aElement);
  void                           DeleteObject(MenuElement *aElement);
  void                           DeleteObjects();
  void                           Update();
  
  virtual void                   ReceiveMessage(Message const& aMessage);
  virtual void                   SendMessage(Message const& aMessage);
  
private:
  void                           ParseFile();
  void                           ParseTransform(GameObject *aObject, ParserNode *aTransform);
  void                           ParseSurface(GameObject *aObject, ParserNode *aSurface);
  void                           ParseEffects(GameObject *aObject, ParserNode *aEffects);
  void                           ParseCustomScript(GameObject *aObject, ParserNode *aCustomScript);
  void                           DeleteElementChildren(MenuElement *aElement);
  void                           ShallowRemoveElementForObject(GameObject *aObject);
  void                           ShallowRemoveElementChildren(MenuElement *aElement);
};

#endif /* MENU_H_ */
