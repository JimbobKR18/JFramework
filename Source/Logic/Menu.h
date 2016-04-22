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
  typedef std::vector<MenuElement*>   ElementContainer;
  typedef ElementContainer::iterator  ElementIT;
  typedef ElementContainer::const_iterator  ConstElementIT;

private:
  Level*            mOwner;
  HashString        mName;
  HashString        mFileName;
  ElementContainer  mMenuElements;

public:
  Menu(Level* aLevel, HashString const &aFileName);
  virtual ~Menu();

  HashString    GetName() const;
  HashString    GetFileName() const;
  Level*        GetLevel() const;
  MenuElement*  GetElement(HashString const &aFileName) const;
  ElementContainer GetElements() const;

  void          AddObject(MenuElement *aElement);
  void          DeleteObject(MenuElement *aElement);
  void          DeleteObjects();
  void          Update();
  
  virtual void  ReceiveMessage(Message const& aMessage);
  virtual void  SendMessage(Message const& aMessage);
  
private:
  void          ParseFile();
};

#endif /* MENU_H_ */
