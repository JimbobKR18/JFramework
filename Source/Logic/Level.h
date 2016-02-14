#ifndef __JFramework_Level_h_
#define __JFramework_Level_h_

#include "Common.h"
#include "GameObject.h"
#include "Parser.h"
#include "TextParser.h"
#include "TileMapGenerator.h"
#include "MathExt.h"

class LevelManager;
class Menu;

class Level
{
public:
  typedef std::vector<GameObject*> ObjectContainer;
  typedef std::vector<Menu*> MenuContainer;
  typedef std::map<HashString, ObjectContainer> FileContainer;
  typedef ObjectContainer::iterator ObjectIT;
  typedef ObjectContainer::const_iterator ConstObjectIT;
  typedef MenuContainer::iterator MenuIT;
  typedef MenuContainer::const_iterator ConstMenuIT;
  typedef FileContainer::iterator FileContainerIT;
  typedef FileContainer::const_iterator ConstFileContainerIT;

private:
  HashString        mName;
  HashString        mFileName;
  HashString        mMusicName;
  ObjectContainer   mObjects;
  ObjectContainer   mStaticObjects;
  MenuContainer     mMenus;
  LevelManager*     mOwner;
  TileMapGenerator* mGenerator;
  GameObject*       mFocusTarget;
  bool              mActive;

  Vector3           mMaxBoundary;
  Vector3           mMinBoundary;
  
  FileContainer     mScenarios;

public:
  Level();
  Level(LevelManager *aManager, HashString const &aFileName, bool aAutoParse);
  virtual ~Level();

  HashString    GetName() const;
  HashString    GetFileName() const;
  HashString    GetMusicName() const;
  LevelManager* GetManager() const;

  TileMapGenerator* GetTileMap() const;
  GameObject*       GetFocusTarget() const;
  GameObject*       FindObject(HashString const &aObjectName);
  ObjectContainer   FindObjects(Vector3 const &aPosition) const;
  Menu*             FindMenu(HashString const &aMenuName);

  void              AddMenu(Menu *aMenu);
  void              RemoveMenu(Menu *aMenu);
  void              RemoveMenus();

  void              AddObject(GameObject *aObject);
  void              AddStaticObject(GameObject *aObject);
  void              DeleteObject(GameObject *aObject);
  GameObject*       CreateObjectDelayed(HashString const &aFileName, HashString const &aFolder = "Game");
  void              DeleteObjectDelayed(GameObject *aObject);
  void              DeleteObjects();
  void              Reset();
  void              ResetLevel();

  void              SetMaxBoundary(Vector3 const &aMaxBoundary);
  void              SetMinBoundary(Vector3 const &aMinBoundary);
  Vector3           GetMaxBoundary() const;
  Vector3           GetMinBoundary() const;

  void              Load(Level* const aPrevLevel);
  void              Unload(Level* const aNextLevel);
  void              LoadObjects(ObjectContainer const &aObjects, bool const aStatic);
  void              UnloadObjects(ObjectContainer const &aObjects);

  virtual void      PreReset() {};
  virtual void      PostReset() {};
  virtual void      Update();
  virtual void      ParseAdditionalData(Root *aRoot, GameObject *aObject);
  virtual void      Serialize(Parser &aParser);
  virtual void      ReceiveMessage(Message const& aMessage);
  virtual void      SendMessage(Message const& aMessage);
  void              SerializeTileMap(Parser &aParser);

  static void       SerializeLUA();

  void              ParseFile(HashString const &aFileName);
  void              LoadScenario(HashString const &aFileName);
  void              UnloadScenario(HashString const &aFileName);
  void              UnloadScenarios();

protected:
  void              ParseBaseFile();
  ObjectContainer&  GetObjects();
  ObjectContainer&  GetStaticObjects();
private:
  void              SerializeObjects(Parser &aParser, ObjectContainer &aObjects, ObjectContainer &aMenuObjects);
  void              SerializeScenarios(Parser &aParser, ObjectContainer &aMenuObjects);
  bool              ObjectNotInScenario(GameObject *aObject);
  void              RemoveObjectFromScenarios(GameObject *aObject);
  void              ParseTransform(GameObject *aObject, Root* aTransform);
  void              ParseSurface(GameObject *aObject, Root* aSurface);
  void              ParsePhysicsObject(GameObject *aObject, Root* aPhysicsObject);
  void              ParseTileGenerator(TextParser &aParser);
};

#endif
