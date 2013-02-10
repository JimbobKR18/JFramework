#ifndef __JFramework_Level_h_
#define __JFramework_Level_h_

#include "Common.h"
#include "GameObject.h"

class LevelManager;
class Menu;

class Level
{
private:
	std::string               mName;
	std::string               mFileName;
	std::vector<GameObject*>  mObjects;
	std::vector<Menu*>        mMenus;
	LevelManager*             mOwner;
	bool                      mActive;
  
  typedef std::vector<GameObject*>::iterator ObjectIT;
  typedef std::vector<Menu*>::iterator MenuIT;
public:
	Level();
	Level(LevelManager *aManager, std::string const &aFileName);
	~Level();

	std::string   GetName() const;
  LevelManager* GetManager() const;
  
  void          AddMenu(Menu *aMenu);
  void          RemoveMenu(Menu *aMenu);

  void          AddObject(GameObject *aObject);
  void          DeleteObject(GameObject *aObject);
  void          DeleteObjects();
  void          Reset();

	void          Load();
	void          Unload();
private:
	void          ParseFile();
};

#endif
