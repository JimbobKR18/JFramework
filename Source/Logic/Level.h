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
public:
  typedef std::vector<GameObject*> ObjectContainer;
  typedef std::vector<Menu*> MenuContainer;
  typedef ObjectContainer::iterator ObjectIT;
  typedef ObjectContainer::const_iterator ConstObjectIT;
  typedef MenuContainer::iterator MenuIT;
  typedef MenuContainer::const_iterator ConstMenuIT;

private:
	std::string       mName;
	std::string       mMusicName;
	ObjectContainer   mObjects;
	ObjectContainer   mStaticObjects;
	MenuContainer     mMenus;
	LevelManager*     mOwner;
	TileMapGenerator* mGenerator;
	GameObject*       mFocusTarget;
	bool              mActive;

	Vector3           mMaxBoundary;
	Vector3           mMinBoundary;

public:
	Level();
	Level(LevelManager *aManager, std::string const &aFileName, bool aAutoParse);
	virtual ~Level();

	std::string   GetName() const;
  LevelManager* GetManager() const;
  
  TileMapGenerator* GetTileMap() const;
  GameObject*       GetFocusTarget() const;
  GameObject*       FindObject(std::string const &aObjectName);
  ObjectContainer   FindObjects(Vector3 const &aPosition) const;
  Menu*             FindMenu(std::string const &aMenuName);

  void              AddMenu(Menu *aMenu);
  void              RemoveMenu(Menu *aMenu);
  void              RemoveMenus();

  void              AddObject(GameObject *aObject);
  void              AddStaticObject(GameObject *aObject);
  void              DeleteObject(GameObject *aObject);
  GameObject*       CreateObjectDelayed(HashString const &aFileName);
  void              DeleteObjectDelayed(GameObject *aObject);
  void              DeleteObjects();
  void              Reset();

  void              SetMaxBoundary(Vector3 const &aMaxBoundary);
  void              SetMinBoundary(Vector3 const &aMinBoundary);
  Vector3           GetMaxBoundary() const;
  Vector3           GetMinBoundary() const;

	void              Load(Level* const aPrevLevel);
	void              Unload(Level* const aNextLevel);

	virtual void      PreReset() {};
	virtual void      Update();
	virtual void      ParseAdditionalData(Root *aRoot, GameObject *aObject) {};
	virtual void      Serialize(Parser &aParser);
	void              SerializeTileMap(Parser &aParser);

	static void       SerializeLUA();

	void              ParseFile();
  
protected:
  ObjectContainer&  GetObjects();
};

#endif
