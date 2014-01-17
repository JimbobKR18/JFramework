#ifndef __JFramework_Level_h_
#define __JFramework_Level_h_

#include "Common.h"
#include "GameObject.h"
#include "Parser.h"
#include "TileMapGenerator.h"
#include "MathExt.h"

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
	TileMapGenerator*         mGenerator;
	GameObject*               mFocusTarget;
	bool                      mActive;

	Vector3                   mMaxBoundary;
	Vector3                   mMinBoundary;
  
  typedef std::vector<GameObject*>::iterator ObjectIT;
  typedef std::vector<Menu*>::iterator MenuIT;
public:
	Level();
	Level(LevelManager *aManager, std::string const &aFileName, bool aAutoParse);
	virtual ~Level();

	std::string   GetName() const;
  LevelManager* GetManager() const;
  
  TileMapGenerator* GetTileMap() const;
  GameObject*       GetFocusTarget() const;
  GameObject*       FindObject(std::string const &aObjectName);

  void              AddMenu(Menu *aMenu);
  void              RemoveMenu(Menu *aMenu);

  void              AddObject(GameObject *aObject);
  void              AddStaticObject(GameObject *aObject);
  void              DeleteObject(GameObject *aObject);
  void              DeleteObjectDelayed(GameObject *aObject);
  void              DeleteObjects();
  void              Reset();

  void              SetMaxBoundary(Vector3 const &aMaxBoundary);
  void              SetMinBoundary(Vector3 const &aMinBoundary);

	void              Load(Level* const aPrevLevel);
	void              Unload(Level* const aNextLevel);

	virtual void      PreReset() {};
	virtual void      Update() {};
	virtual void      ParseAdditionalData(Parser *aParser, GameObject *aObject, std::string const &aFirstParam) {};

	static void       SerializeLUA();

	void              ParseFile();
};

#endif
