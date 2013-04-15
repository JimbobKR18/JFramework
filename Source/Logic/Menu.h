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
  Level*                                      mOwner;
  std::string                                 mFilename;
  std::vector<MenuElement*>                   mMenuElements;
  typedef std::vector<MenuElement*>::iterator ElementIT;

public:
  Menu(Level* aLevel, std::string const &aFilename);
  ~Menu();

  std::string   GetName();
  Level*        GetLevel();

  void          AddObject(MenuElement *aElement);
  void          DeleteObject(MenuElement *aElement);
  void          DeleteObjects();
  
private:
  void          ParseFile();
};

#endif /* MENU_H_ */
