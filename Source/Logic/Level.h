#ifndef __JFramework_Level_h_
#define __JFramework_Level_h_

#include "Common.h"
#include "GameObject.h"
#include "Parser.h"
#include "TileMapGenerator.h"
#include "MathExt.h"
#include "Camera.h"

class LevelManager;
class Level
{
public:
  typedef std::unordered_set<GameObject*> ObjectContainer;
  typedef std::vector<GameObject*> ObjectVector;
  
  // File Name -> Object Container
  typedef std::map<HashString, ObjectContainer> FileContainer;
  // Sound Name -> Group Name
  typedef std::map<HashString, HashString> SoundNameContainer;
  // Sound Channel -> Sound Name
  typedef std::map<int, HashString> SoundChannelContainer;
  typedef std::set<HashString> SoundBankContainer;
  typedef std::vector<ObjectContainer> ObjectContainerMap;
  typedef std::vector<TileMapGenerator*> TileMapGeneratorContainer;
  typedef ObjectContainer::iterator ObjectIT;
  typedef ObjectContainer::const_iterator ConstObjectIT;
  typedef TileMapGeneratorContainer::iterator TileMapGeneratorContainerIT;
  typedef TileMapGeneratorContainer::const_iterator ConstTileMapGeneratorContainerIT;
  typedef FileContainer::iterator FileContainerIT;
  typedef FileContainer::const_iterator ConstFileContainerIT;
  typedef SoundChannelContainer::iterator SoundChannelContainerIT;
  typedef SoundChannelContainer::const_iterator ConstSoundChannelContainerIT;
  typedef SoundNameContainer::iterator SoundNameContainerIT;
  typedef SoundNameContainer::const_iterator ConstSoundNameContainerIT;
  typedef SoundBankContainer::iterator SoundBankContainerIT;
  typedef SoundBankContainer::const_iterator ConstSoundBankContainerIT;

private:
  HashString                 mName;
  HashString                 mFolderName;
  HashString                 mFileName;
  SoundNameContainer         mSoundNames;
  ObjectContainerMap         mObjects;
  LevelManager*              mOwner;
  TileMapGeneratorContainer  mGenerators;
  GameObject*                mFocusTarget;
  Vector4                    mClearColor;
  SoundBankContainer         mSoundBanks;
  SoundChannelContainer      mSoundChannels;
  std::unordered_set<int>    mUnsortedLayers;

  Vector3                    mMaxBoundary;
  Vector3                    mMinBoundary;
  
  FileContainer              mScenarios;

public:
  Level();
  Level(LevelManager *aManager, HashString const &aFileName, HashString const &aFolderName = "Game", bool aAutoParse = true);
  virtual ~Level();

  HashString    GetName() const;
  HashString    GetFileName() const;
  SoundNameContainer GetSoundNames() const;
  LevelManager* GetManager() const;
  SoundChannelContainer GetSoundChannels() const;
  SoundBankContainer GetSoundBanks() const;

  TileMapGenerator* GetTileMap(int const aIndex = 0) const;
  GameObject*       GetFocusTarget() const;
  void              SetFocusTarget(GameObject *aObject);
  GameObject*       FindObject(HashString const &aObjectName);
  ObjectVector      FindObjects(HashString const &aName);
  ObjectVector      FindObjects(Vector3 const &aPosition) const;

  void              AddObject(GameObject *aObject, ObjectPlacement const aPlacement);
  virtual void      DeleteObject(GameObject *aObject);
  GameObject*       CreateObjectDelayed(HashString const &aFileName, HashString const &aFolder = "Game", HashString const &aType = "Default", ObjectPlacement const aPlacement = ObjectPlacement::DEFAULT);
  virtual void      DeleteObjectDelayed(GameObject *aObject);
  virtual void      DeleteObjects();
  void              Reset();
  void              ResetLevel();

  void              SetMaxBoundary(Vector3 const &aMaxBoundary);
  void              SetMinBoundary(Vector3 const &aMinBoundary);
  Vector3           GetMaxBoundary() const;
  Vector3           GetMinBoundary() const;

  void              Load(Level const *aPrevLevel);
  void              Unload(Level *aNextLevel);
  void              LoadObjects(ObjectContainer const &aObjects, ObjectPlacement const aPlacement);
  void              UnloadObjects(ObjectContainer const &aObjects);

  virtual void      PreReset() {};
  virtual void      PostReset() {};
  virtual void      Update();
  virtual void      ParseAdditionalData(ParserNode *aRoot, GameObject *aObject);
  virtual void      Serialize(Parser &aParser);
  virtual void      ReceiveMessage(Message const& aMessage);
  virtual void      SendMessage(Message const& aMessage);

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
  void              LoadObject(GameObject *aObject, ObjectPlacement const aPlacement);
  void              UnloadObject(GameObject *aObject);
  void              DeleteObjectChildren(GameObject *aObject);
  void              DeleteObjectChildrenDelayed(GameObject *aObject);
  void              SerializeObjects(Parser &aParser, ObjectContainer &aObjects);
  void              SerializeScenarios(Parser &aParser);
  bool              ObjectNotInScenario(GameObject *aObject);
  void              RemoveObjectFromScenarios(GameObject *aObject);
  void              ParseTransform(GameObject *aObject);
  void              ParseEffects(GameObject *aObject, ParserNode *aEffects);
  void              ParseCustomScript(GameObject *aObject, ParserNode *aCustomScript);
  void              ParseTileGenerator(ParserNode *aTileMap);
};

#endif
