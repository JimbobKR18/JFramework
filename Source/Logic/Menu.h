/*
 * Menu.h
 *
 *  Created on: Feb 3, 2013
 *      Author: jimbob
 */

#ifndef __JFramework_Menu_h_
#define __JFramework_Menu_h_

#include "GameObject.h"
#include "Level.h"

class Menu
{
private:
  Level*                                      mOwner;
  std::string                                 mFilename;
  std::vector<GameObject*>                    mObjects;
  typedef std::vector<GameObject*>::iterator  ObjectIT;

public:
  Menu(Level* aLevel, std::string const &aFilename);
  ~Menu();

  std::string   GetName();
  Level*        GetLevel();

  void          AddObject(GameObject *aObject);
  void          DeleteObject(GameObject *aObject);
  void          DeleteObjects();
};

#endif /* MENU_H_ */
