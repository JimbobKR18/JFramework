#ifndef __JFramework_Level_h_
#define __JFramework_Level_h_

#include "Common.h"
#include "GameObject.h"
#include "Parser.h"
#include "TextParser.h"
#include "TileMapGenerator.h"
#include "MathExt.h"

enum ObjectPlacement
{
  DEFAULT = 0,
  REPLACEABLE,
  STATIC,
  PLACEMENT_ALL
};

class Menu;
class LevelManager;
class Level
{
public:
  typedef std::set<GameObject*> ObjectContainer;
  typedef std::vector<GameObject*> ObjectVector;
  typedef std::map<HashString, ObjectContainer> FileContainer;
  typedef std::vector<Menu*> MenuContainer;
  typedef ObjectContainer::iterator ObjectIT;
  typedef ObjectContainer::const_iterator ConstObjectIT;
  typedef MenuContainer::iterator MenuIT;
  typedef MenuContainer::const_iterator ConstMenuIT;
  typedef FileContainer::iterator FileContainerIT;
  typedef FileContainer::const_iterator ConstFileContainerIT;
  typedef std::vector<ObjectContainer> ObjectContainerMap;

private:
  HashString         mName;
  HashString         mFolderName;
  HashString         mFileName;
  HashString         mMusicName;
  ObjectContainerMap mObjects;
  MenuContainer      mMenus;
  LevelManager*      mOwner;
  TileMapGenerator*  mGenerator;
  GameObject*        mFocusTarget;
  Vector4            mClearColor;
  int                mMusicChannel;

  Vector3            mMaxBoundary;
  Vector3            mMinBoundary;
  
  FileContainer      mScenarios;

public:
  Level();
  Level(LevelManager *aManager, HashString const &aFileName, HashString const &aFolderName = "Game", bool aAutoParse = true);
  virtual ~Level();

  HashString    GetName() const;
  HashString    GetFileName() const;
  HashString    GetMusicName() const;
  LevelManager* GetManager() const;
  int           GetMusicChannel() const;

  TileMapGenerator* GetTileMap() const;
  GameObject*       GetFocusTarget() const;
  GameObject*       FindObject(HashString const &aObjectName);
  ObjectVector      FindObjects(Vector3 const &aPosition) const;
  Menu*             FindMenu(HashString const &aMenuName);
  
  void              AddMenu(Menu *aMenu);
  void              RemoveMenu(Menu *aMenu);
  void              RemoveMenus();

  void              AddObject(GameObject *aObject, ObjectPlacement const aPlacement);
  void              DeleteObject(GameObject *aObject);
  GameObject*       CreateObjectDelayed(HashString const &aFileName, HashString const &aFolder = "Game", HashString const &aType = "Default", ObjectPlacement const aPlacement = ObjectPlacement::DEFAULT);
  void              DeleteObjectDelayed(GameObject *aObject);
  void              DeleteObjects();
  void              Reset();
  void              ResetLevel();

  void              SetMaxBoundary(Vector3 const &aMaxBoundary);
  void              SetMinBoundary(Vector3 const &aMinBoundary);
  Vector3           GetMaxBoundary() const;
  Vector3           GetMinBoundary() const;

  void              Load(Level const *aPrevLevel);
  void              Unload(Level const *aNextLevel);
  void              LoadObjects(ObjectContainer const &aObjects, ObjectPlacement const aPlacement);
  void              UnloadObjects(ObjectContainer const &aObjects);

  virtual void      PreReset() {};
  virtual void      PostReset() {};
  virtual void      Update();
  virtual void      ParseAdditionalData(ParserNode *aRoot, GameObject *aObject);
  virtual void      Serialize(Parser &aParser);
  virtual void      ReceiveMessage(Message const& aMessage);
  virtual void      SendMessage(Message const& aMessage);
  void              SerializeTileMap(Parser &aParser);

  static void       SerializeLUA();

  void              ParseFile(HashString const &aFileName, HashString const &aFolderName = "Game");
  ObjectContainer*  CreateEmptyScenario(HashString const &aScenarioName);
  ObjectContainer*  ParseAndLoadScenario(HashString const &aFileName, HashString const &aFolderName = "Game");
  void              LoadScenario(HashString const &aFileName);
  void              UnloadScenario(HashString const &aFileName);
  ObjectContainer*  GetScenario(HashString const &aFileName);
  void              UnloadScenarios();

protected:
  void              ParseBaseFile();
  ObjectContainer&  GetObjects(ObjectPlacement const aPlacement);
  GameObject*       FindObject(ObjectContainer const &aContainer, HashString const &aName);
private:
  void              DeleteObjectChildren(GameObject *aObject);
  void              DeleteObjectChildrenDelayed(GameObject *aObject);
  void              SerializeObjects(Parser &aParser, ObjectContainer &aObjects, ObjectContainer &aMenuObjects);
  void              SerializeScenarios(Parser &aParser, ObjectContainer &aMenuObjects);
  bool              ObjectNotInScenario(GameObject *aObject);
  void              RemoveObjectFromScenarios(GameObject *aObject);
  void              RemoveObjectFromMenus(GameObject *aObject);
  void              ParseTransform(GameObject *aObject, ParserNode* aTransform);
  void              ParseSurface(GameObject *aObject, ParserNode* aSurface);
  void              ParseText(GameObject *aObject, ParserNode* aText);
  void              ParsePhysicsObject(GameObject *aObject, ParserNode* aPhysicsObject);
  void              ParseChemistryMaterial(GameObject *aObject, ParserNode* aChemistryMaterial);
  void              ParseChemistryElement(GameObject *aObject, ParserNode* aChemistryElement);
  void              ParseEffects(GameObject *aObject, ParserNode *aEffects);
  void              ParseCustomScript(GameObject *aObject, ParserNode *aCustomScript);
  void              ParseTileGenerator(TextParser &aParser);
};

#endif
