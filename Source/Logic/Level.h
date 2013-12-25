#ifndef __JFramework_Level_h_
#define __JFramework_Level_h_

#include "Common.h"
#include "GameObject.h"
#include "Parser.h"

class LevelManager;
class Menu;

class Level
{
private:
	std::string               mName;
	std::string               mFileName;
	std::string               mMusicName;
	std::vector<GameObject*>  mObjects;
	std::vector<GameObject*>  mStaticObjects;
	std::vector<Menu*>        mMenus;
	LevelManager*             mOwner;
	GameObject*               mFocusTarget;
	bool                      mActive;
  
  typedef std::vector<GameObject*>::iterator ObjectIT;
  typedef std::vector<Menu*>::iterator MenuIT;
public:
	Level();
	Level(LevelManager *aManager, std::string const &aFileName);
	virtual ~Level();

	std::string   GetName() const;
  LevelManager* GetManager() const;
  
  void          AddMenu(Menu *aMenu);
  void          RemoveMenu(Menu *aMenu);

  void          AddObject(GameObject *aObject);
  void          AddStaticObject(GameObject *aObject);
  void          DeleteObject(GameObject *aObject);
  void          DeleteObjectDelayed(GameObject *aObject);
  void          DeleteObjects();
  void          Reset();

	void          Load();
	void          Unload();

	virtual void  PreReset() {};
	virtual void  Update() {};
	virtual void  ParseAdditionalData(Parser *aParser, GameObject *aObject, std::string const &aFirstParam) {};

	static void   SerializeLUA();
private:
	void          ParseFile();
};

#endif
