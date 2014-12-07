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

private:
  Level*            mOwner;
  HashString        mName;
  HashString        mFileName;
  ElementContainer  mMenuElements;

public:
  Menu(Level* aLevel, std::string const &aFileName);
  virtual ~Menu();

  HashString    GetName();
  HashString    GetFileName();
  Level*        GetLevel();
  MenuElement*  GetElement(HashString const &aFileName);

  void          AddObject(MenuElement *aElement);
  void          DeleteObject(MenuElement *aElement);
  void          DeleteObjects();
  
  virtual void  ReceiveMessage(Message const& aMessage);
  virtual void  SendMessage(Message const& aMessage);
  
private:
  void          ParseFile();
};

#endif /* MENU_H_ */
