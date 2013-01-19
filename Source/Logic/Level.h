#ifndef __JFramework_Level_h_
#define __JFramework_Level_h_

#include "Common.h"
#include "GameObject.h"

class LevelManager;

class Level
{
private:
	std::string               mName;
	std::string               mFileName;
	std::vector<GameObject*>  mObjects;
	LevelManager*             mOwner;
	bool                      mActive;
  
  typedef std::vector<GameObject*>::iterator ObjectIT;
public:
	Level();
	Level(LevelManager *aManager, std::string const &aFileName);
	~Level();

	std::string GetName() const;
  LevelManager *GetManager() const;
  
  void AddObject(GameObject *aObject);
  void DeleteObject(GameObject *aObject);
  void DeleteObjects();

	void Load();
	void Unload();
private:
	void ParseFile();
};

#endif
