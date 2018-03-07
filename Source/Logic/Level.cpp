#include "Level.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "MathExt.h"
#include "Transform.h"
#include "ChemistryMaterial.h"
#include "ChemistryElement.h"
#include "GraphicsManager.h"
#include "ChemistryManager.h"
#include "CustomScript.h"
#include "SoundEmitter.h"
#include "SoundListener.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "ControllerManager.h"
#include "SoundManager.h"
#include "TileMapGenerator.h"
#include "Common.h"
#include "LuaIncludes.h"
#include "ObjectDeleteMessage.h"
#include "ObjectCreateMessage.h"
#include "ResetLevelMessage.h"
#include "EffectsManager.h"
#include "ParserFactory.h"
#include "FollowComponent.h"

#if !defined(IOS) && !defined(ANDROID)
  #define SHADER_COMPATIBLE
#endif

bool SortPredicate(GameObject *aLhs, GameObject *aRhs)
{
  return aLhs->GET<Transform>()->GetPosition().z <= aRhs->GET<Transform>()->GetPosition().z;
}

Level::Level()
{
  assert(!"Do not use the level default constructor.");
}

Level::Level(LevelManager *aManager, HashString const &aFileName, HashString const &aFolderName, bool aAutoParse) :
             mName(""), mFolderName(aFolderName), mFileName(aFileName), mMusicNames(), mObjects(),
             mOwner(aManager), mGenerators(), mFocusTarget(nullptr), mClearColor(0, 0, 0, 1), mMusicChannels(),
             mMaxBoundary(0,0,0), mMinBoundary(0,0,0), mScenarios()
{
  for(int i = static_cast<int>(aFileName.Size()) - 1;
      i >= 0 && aFileName[i] != '/'; --i)
  {
    mName.Push(aFileName[i]);
  }
  mName.Reverse();
  mName = mName.SubString(0, mName.Size() - 4);
  
  for(int i = ObjectPlacement::DEFAULT; i != ObjectPlacement::PLACEMENT_ALL; ++i)
  {
    mObjects.push_back(ObjectContainer());
  }

  if(aAutoParse)
  {
    ParseBaseFile();
  }
}

Level::~Level()
{
  DeleteObjects();
  for(TileMapGeneratorContainerIT it = mGenerators.begin(); it != mGenerators.end(); ++it)
    delete *it;
  mGenerators.clear();
}

/**
 * @brief Get the level name. Not the file name.
 * @return The level name.
 */
HashString Level::GetName() const
{
  return mName;
}

/**
* @brief Get the file name, not the level name.
* @return The file name.
*/
HashString Level::GetFileName() const
{
  return mFileName;
}

/**
* @brief Get the music names.
* @return The music names.
*/
Level::MusicNameContainer Level::GetMusicNames() const
{
  return mMusicNames;
}

/**
 * @brief Get the level manager.
 * @return The level manager.
 */
LevelManager *Level::GetManager() const
{
  return mOwner;
}

/**
 * @brief Get music channels
 * @return Music channels
 */
Level::MusicChannelContainer Level::GetMusicChannels() const
{
  return mMusicChannels;
}

/**
 * @brief Get the tile map generator. You can get individual tiles and other helperful things here.
 * @param aIndex Which tile map to get.
 * @return The tile map generator.
 */
TileMapGenerator* Level::GetTileMap(int const aIndex) const
{
  if(aIndex < 0 || aIndex >= mGenerators.size())
  {
    DebugLogPrint("TileMapGenerator index %d out of bounds. (Level.cpp GetTileMap)", aIndex);
    assert(!"TileMapGenerator index out of bounds. (Level.cpp GetTileMap)");
  }
    
  return mGenerators[aIndex];
}

/**
 * @brief Get the level's focus target for the level.
 * @return The focus target.
 */
GameObject* Level::GetFocusTarget() const
{
  return mFocusTarget;
}

/**
 * @brief Set the level's focus target for the level.
 * @param aObject The focus target.
 */
void Level::SetFocusTarget(GameObject *aObject)
{
  mFocusTarget = aObject;
}

/**
 * @brief Find an object by name.
 * @param aObjectName
 * @return The object, or nullptr.
 */
GameObject* Level::FindObject(HashString const &aObjectName)
{
  // This is designed to be naive
  // if two objects share the same name...
  // well, too bad.
  for(int i = ObjectPlacement::DEFAULT; i != ObjectPlacement::PLACEMENT_ALL; ++i)
  {
    ObjectIT end = mObjects[i].end();
    for(ObjectIT it = mObjects[i].begin(); it != end; ++it)
    {
      if(aObjectName == (*it)->GetName() || aObjectName == (*it)->GetFileName())
        return *it;
    }
  }
  return nullptr;
}

/**
 * @brief Find all objects touching a position
 * @param aPosition
 * @return A vector of objects touching the location, can be empty.
 */
Level::ObjectVector Level::FindObjects(Vector3 const &aPosition) const
{
  ObjectVector ret;
  for(ConstObjectIT it = mObjects[ObjectPlacement::DEFAULT].begin(); it != mObjects[ObjectPlacement::DEFAULT].end(); ++it)
  {
    Transform* transform = (*it)->GET<Transform>();
    AxisAlignedBoundingBox cube(transform->GetPosition(), transform->GetSize());

    if(cube.Get2DCollision(aPosition))
    {
      ret.push_back(*it);
    }
  }
  std::sort(ret.begin(), ret.end(), SortPredicate);
  return ret;
}

/**
 * @brief Add object to our vector of objects.
 * @param aObject
 */
void Level::AddObject(GameObject *aObject, ObjectPlacement const aPlacement)
{
  mObjects[aPlacement].insert(aObject);
}

/**
 * @brief Delete object and remove from our objects vector.
 * @param aObject Object to delete.
 */
void Level::DeleteObject(GameObject *aObject)
{
  ObjectManager *objectManager = mOwner->GetOwningApp()->GET<ObjectManager>();
  EffectsManager *effectsManager = mOwner->GetOwningApp()->GET<EffectsManager>();
  
  for(int i = ObjectPlacement::DEFAULT; i != ObjectPlacement::PLACEMENT_ALL; ++i)
  {
    ObjectIT end = mObjects[i].end();
    for(ObjectIT it = mObjects[i].begin(); it != end; ++it)
    {
      if(aObject == *it)
      {
        RemoveObjectFromScenarios(*it);
        mObjects[i].erase(it);
        DeleteObjectChildren(aObject);
        objectManager->DeleteObject(aObject);
        effectsManager->RemoveEffectsForObject(aObject);
        return;
      }
    }
  }
}

/**
 * @brief Create an object to be added next frame.
 * @param aFileName
 * @return The newly created object.
 */
GameObject* Level::CreateObjectDelayed(HashString const &aFileName, HashString const &aFolder, HashString const &aType, ObjectPlacement const aPlacement)
{
  ObjectManager *objectManager = mOwner->GetOwningApp()->GET<ObjectManager>();
  GameObject *object = objectManager->CreateObjectNoAdd(aFileName, aFolder, aType);
  ObjectCreateMessage *msg = new ObjectCreateMessage(object);
  objectManager->ProcessDelayedMessage(msg);
  AddObject(object, aPlacement);
  return object;
}

/**
 * @brief Marks an object for deletion next frame.
 * @param aObject Object to delete.
 */
void Level::DeleteObjectDelayed(GameObject *aObject)
{
  ObjectManager *objectManager = mOwner->GetOwningApp()->GET<ObjectManager>();
  GraphicsManager *graphicsManager = mOwner->GetOwningApp()->GET<GraphicsManager>();
  EffectsManager *effectsManager = mOwner->GetOwningApp()->GET<EffectsManager>();
  bool found = false;
  
  for(int i = ObjectPlacement::DEFAULT; i != ObjectPlacement::PLACEMENT_ALL; ++i)
  {
    ObjectIT end = mObjects[i].end();
    for(ObjectIT it = mObjects[i].begin(); it != end; ++it)
    {
      if(aObject == *it)
      {
        RemoveObjectFromScenarios(*it);
        mObjects[i].erase(it);
        DeleteObjectChildrenDelayed(aObject);
        
        ObjectDeleteMessage *msg = new ObjectDeleteMessage(aObject);
        objectManager->ProcessDelayedMessage(msg);
        effectsManager->RemoveEffectsForObject(aObject);
        found = true;
        break;
      }
      if(found)
        break;
    }
  }
  ObjectDeleteMessage *msg = new ObjectDeleteMessage(aObject);
  graphicsManager->ProcessDelayedMessage(msg);
}

/**
 * @brief Delete all objects in level.
 */
void Level::DeleteObjects()
{
  ObjectManager *manager = mOwner->GetOwningApp()->GET<ObjectManager>();
  for(int i = ObjectPlacement::DEFAULT; i != ObjectPlacement::PLACEMENT_ALL; ++i)
  {
    ObjectIT end = mObjects[i].end();
    for(ObjectIT it = mObjects[i].begin(); it != end; ++it)
    {
      RemoveObjectFromScenarios(*it);
      manager->DeleteObject(*it);
    }
    mObjects[i].clear();
  }
  manager->GetOwningApp()->ClearDelayedMessages();
}

/**
 * @brief Delete objects and start level over.
 */
void Level::Reset()
{
  GetManager()->ProcessDelayedMessage(new ResetLevelMessage(mName));
}

/**
 * @brief Reset level helper
 */
void Level::ResetLevel()
{
  PreReset();
  // Reset view to set when reparsing file.
  GraphicsManager *graphicsManager = GetManager()->GetOwningApp()->GET<GraphicsManager>();
  graphicsManager->SetPrimaryCamera(nullptr);
  
  // Delete tile maps.
  for(TileMapGeneratorContainerIT it = mGenerators.begin(); it != mGenerators.end(); ++it)
    delete *it;
  mGenerators.clear();
  
  // Delete objects and reload.
  DeleteObjects();
  ParseBaseFile();
  PostReset();
}

/**
 * @brief Set max area for camera view.
 * @param aMaxBoundary
 */
void Level::SetMaxBoundary(Vector3 const &aMaxBoundary)
{
  mMaxBoundary = aMaxBoundary;
}

/**
 * @brief Set min area for camera view.
 * @param aMinBoundary
 */
void Level::SetMinBoundary(Vector3 const &aMinBoundary)
{
  mMinBoundary = aMinBoundary;
}

/**
 * @brief Get the max boundary of the level.
 * @return The max boundary of the level.
 */
Vector3 Level::GetMaxBoundary() const
{
  return mMaxBoundary;
}

/**
 * @brief Get the min boundary of the level.
 * @return The min boundary of the level.
 */
Vector3 Level::GetMinBoundary() const
{
  return mMinBoundary;
}

/**
 * @brief Place all objects to be rendered / updated.
 * @param aPrevLevel
 */
void Level::Load(Level const *aPrevLevel)
{
  // Load all objects
  for(int i = ObjectPlacement::DEFAULT; i != ObjectPlacement::PLACEMENT_ALL; ++i)
    LoadObjects(mObjects[i], static_cast<ObjectPlacement>(i));

  for(MusicNameContainerIT it = mMusicNames.begin(); it != mMusicNames.end(); ++it)
  {
    if(!aPrevLevel || aPrevLevel->mMusicNames.find(*it) == aPrevLevel->mMusicNames.end())
    {
      int channel = mOwner->GetOwningApp()->GET<SoundManager>()->PlaySound(*it, SoundManager::INFINITE_LOOPS);
      mMusicChannels[channel] = *it;
    }
  }

  mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->SetClearColor(mClearColor);
  
  Camera *primaryCamera = mOwner->GetOwningApp()->GET<GraphicsManager>()->GetPrimaryCamera();
  primaryCamera->GetOwner()->GET<Transform>()->SetMaxBoundary(mMaxBoundary);
  primaryCamera->GetOwner()->GET<Transform>()->SetMinBoundary(mMinBoundary);
}

/**
 * @brief Specify all objects to be be displayed / updated.
 * @param aNextLevel
 */
void Level::Unload(Level *aNextLevel)
{
  // Unload all objects
  for(int i = ObjectPlacement::DEFAULT; i != ObjectPlacement::PLACEMENT_ALL; ++i)
    UnloadObjects(mObjects[i]);
    
  for(MusicChannelContainerIT it = mMusicChannels.begin(); it != mMusicChannels.end(); ++it)
  {
    if(!aNextLevel || aNextLevel->mMusicNames.find(it->second) == aNextLevel->mMusicNames.end())
      mOwner->GetOwningApp()->GET<SoundManager>()->StopChannel(it->first);
    else if(aNextLevel)
      aNextLevel->mMusicChannels[it->first] = it->second;
  }
  mMusicChannels.clear();

  mOwner->GetOwningApp()->GET<GraphicsManager>()->SetPrimaryCamera(nullptr);
}

/**
 * @brief Load all objects in list into view.
 * @param aObjects Objects to add.
 */
void Level::LoadObjects(ObjectContainer const &aObjects, ObjectPlacement const aPlacement)
{
  for(ConstObjectIT it = aObjects.begin(); it != aObjects.end(); ++it)
  {
    mOwner->GetOwningApp()->GET<ObjectManager>()->AddObject(*it, (aPlacement == ObjectPlacement::STATIC) ? true : false);
    if((*it)->GET<PhysicsObject>())
      mOwner->GetOwningApp()->GET<PhysicsWorld>()->AddObject((*it)->GET<PhysicsObject>());
    if((*it)->GET<Surface>())
      mOwner->GetOwningApp()->GET<GraphicsManager>()->AddSurface((*it)->GET<Surface>());
    if((*it)->GET<Controller>())
      mOwner->GetOwningApp()->GET<ControllerManager>()->AddController((*it)->GET<Controller>());
    if((*it)->GET<ChemistryMaterial>())
      mOwner->GetOwningApp()->GET<ChemistryManager>()->AddMaterial((*it)->GET<ChemistryMaterial>());
    if((*it)->GET<ChemistryElement>())
      mOwner->GetOwningApp()->GET<ChemistryManager>()->AddElement((*it)->GET<ChemistryElement>());
    if((*it)->GET<Camera>())
      mOwner->GetOwningApp()->GET<GraphicsManager>()->AddCamera((*it)->GET<Camera>());
    if((*it)->GET<Camera>() && (*it)->GET<Camera>()->GetPrimary())
      mOwner->GetOwningApp()->GET<GraphicsManager>()->SetPrimaryCamera((*it)->GET<Camera>());
    if((*it)->GET<FollowComponent>())
      (*it)->GET<FollowComponent>()->ResetTarget();
  }
}

/**
 * @brief Unload all objects in the list from view, they will still exist in the level file.
 * @param aObjects Objects to unload.
 */
void Level::UnloadObjects(ObjectContainer const &aObjects)
{
  for(ConstObjectIT it = aObjects.begin(); it != aObjects.end(); ++it)
  {
    // Remove all components
    mOwner->GetOwningApp()->GET<ObjectManager>()->RemoveObject(*it);
    if((*it)->GET<PhysicsObject>())
      mOwner->GetOwningApp()->GET<PhysicsWorld>()->RemoveObject((*it)->GET<PhysicsObject>());
    if((*it)->GET<Surface>())
      mOwner->GetOwningApp()->GET<GraphicsManager>()->RemoveSurface((*it)->GET<Surface>());
    if((*it)->GET<Controller>())
      mOwner->GetOwningApp()->GET<ControllerManager>()->RemoveController((*it)->GET<Controller>());
    if((*it)->GET<ChemistryMaterial>())
      mOwner->GetOwningApp()->GET<ChemistryManager>()->RemoveMaterial((*it)->GET<ChemistryMaterial>());
    if((*it)->GET<ChemistryElement>())
      mOwner->GetOwningApp()->GET<ChemistryManager>()->RemoveElement((*it)->GET<ChemistryElement>());
    if((*it)->GET<Camera>())
      mOwner->GetOwningApp()->GET<GraphicsManager>()->RemoveCamera((*it)->GET<Camera>());
    if((*it)->GET<SoundEmitter>())
      (*it)->GET<SoundEmitter>()->StopSound();
  }
}

/**
 * @brief Update loop for levels. LevelManager will call this for you.
 */
void Level::Update()
{
  for(ObjectIT it = mObjects[ObjectPlacement::REPLACEABLE].begin(); it != mObjects[ObjectPlacement::REPLACEABLE].end();)
  {
    DeleteObject(*it);
    it = mObjects[ObjectPlacement::REPLACEABLE].begin();
  }
  mObjects[ObjectPlacement::REPLACEABLE].clear();
  
  // Update tile animations if possible
  for(TileMapGeneratorContainerIT it = mGenerators.begin(); it != mGenerators.end(); ++it)
    (*it)->Update();
}

/**
 * @brief Any extra nodes inside of a level file need to be parsed, but if we reach here then you are autoparsing a level file that derives from level.
 * @param aRoot Root node to read from.
 * @param aObject Object to apply data to.
 */
void Level::ParseAdditionalData(ParserNode *aRoot, GameObject *aObject)
{ 
  aObject->ParseAdditionalData(aRoot);
}

/**
 * @brief Serialize level to file.
 * @param aParser
 */
void Level::Serialize(Parser &aParser)
{
  // Place tile map generators.
  int index = 0;
  aParser.GetBaseRoot()->Place("TileMapGenerators", "");
  ParserNode *tileMapGenerators = aParser.GetBaseRoot()->Find("TileMapGenerators");
  for(TileMapGeneratorContainerIT it = mGenerators.begin(); it != mGenerators.end(); ++it, ++index)
  {
    HashString tileMapGenerator = "TileMapGenerator_" + Common::IntToString(index);
    tileMapGenerators->Place(tileMapGenerator, "");
    (*it)->Serialize(tileMapGenerators->Find(tileMapGenerator));
  }
    
  // For each object not in a scenario, place in default scenario.
  for(ObjectIT it = mObjects[ObjectPlacement::DEFAULT].begin(); it != mObjects[ObjectPlacement::DEFAULT].end(); ++it)
  {
    if(ObjectNotInScenario(*it))
      mScenarios[mFileName].insert(*it);
  }
    
  SerializeObjects(aParser, mScenarios[mFileName]);
  SerializeScenarios(aParser);
  
  // Music
  index = 0;
  aParser.Place("Music", "");
  for(MusicNameContainerIT it = mMusicNames.begin(); it != mMusicNames.end(); ++it, ++index)
  {
    aParser.Place("Music", "Song_" + Common::IntToString(index), *it);
  }
  
  // Clear color
  aParser.Place("ClearColor", "");
  aParser.Place("ClearColor", "ColorR", Common::FloatToString(mClearColor.x));
  aParser.Place("ClearColor", "ColorG", Common::FloatToString(mClearColor.y));
  aParser.Place("ClearColor", "ColorB", Common::FloatToString(mClearColor.z));
  aParser.Place("ClearColor", "ColorA", Common::FloatToString(mClearColor.w));
}

/**
 * @brief Receive message loop
 * @param aMessage message to receive
 */
void Level::ReceiveMessage(Message const& aMessage) 
{
  if(aMessage.GetDescription() == "ResetLevel")
  {
    ResetLevel();
  }
}

/**
 * @brief Send message to manager
 * @param aMessage Message to send
 */
void Level::SendMessage(Message const& aMessage) 
{
  GetManager()->SendMessage(aMessage);
}

/**
 * @brief Make this object viewable in LUA.
 */
void Level::SerializeLUA()
{
  SLB::Class<Level>("Level")
          .set("Load", &Level::Load)
          .set("Unload", &Level::Unload)
          .set("GetName", &Level::GetName)
          .set("GetFileName", &Level::GetFileName)
          .set("CreateObject", &Level::CreateObjectDelayed)
          .set("DeleteObject", &Level::DeleteObjectDelayed)
          .set("ParseFile", &Level::ParseFile)
          .set("ParseAndLoadScenario", &Level::ParseAndLoadScenario)
          .set("LoadScenario", &Level::LoadScenario)
          .set("UnloadScenario", &Level::UnloadScenario)
          .set("UnloadScenarios", &Level::UnloadScenarios);
}

/**
 * @brief Parse file to create our level. Include game object component overrides.
 */
void Level::ParseFile(HashString const &aFileName, HashString const &aFolderName)
{
  Parser *parser = ParserFactory::CreateInputParser(aFolderName, aFileName);
  GameObject *object = nullptr;
  HashString const curObject = "Object_";
  int curIndex = 0;

  HashString tempIndex = curObject + Common::IntToString(curIndex);
  ParserNode* curRoot = parser->Find(tempIndex);
  
  ObjectManager *objectManager = mOwner->GetOwningApp()->GET<ObjectManager>();
  GraphicsManager *graphicsManager = mOwner->GetOwningApp()->GET<GraphicsManager>();

  // While there are objects to find.
  while(curRoot)
  {
    // Make Object to assign params to
    HashString folder = "Game";
    HashString type = "Type";
    
    if(curRoot->Find("Folder"))
      folder = curRoot->Find("Folder")->GetValue();
    if(curRoot->Find("Type"))
      type = curRoot->Find("Type")->GetValue();
    
    object = objectManager->CreateObjectNoAdd(curRoot->Find("File")->GetValue(), folder, type);
    mObjects[ObjectPlacement::DEFAULT].insert(object);
    mScenarios[aFileName].insert(object);

    // Get transform information
    if(curRoot->Find("Transform"))
    {
      ParseTransform(object, curRoot->Find("Transform"));
    }
    // Get surface information
    if(curRoot->Find("Surface"))
    {
      ParseSurface(object, curRoot->Find("Surface"));
    }
    // Get text information (still a surface)
    if(curRoot->Find("Text"))
    {
      ParseText(object, curRoot->Find("Text"));
    }
    // Get physics information
    if(curRoot->Find("PhysicsObject"))
    {
      ParsePhysicsObject(object, curRoot->Find("PhysicsObject"));
    }
    // Get effects information
    if(curRoot->Find("Effects"))
    {
      ParseEffects(object, curRoot->Find("Effects"));
    }
    // Get customscript information
    if(curRoot->Find("CustomScript"))
    {
      ParseCustomScript(object, curRoot->Find("CustomScript"));
    }
    // Get chemistry information
    if(curRoot->Find("ChemistryMaterial"))
    {
      ParseChemistryMaterial(object, curRoot->Find("ChemistryMaterial"));
    }
    if(curRoot->Find("ChemistryElement"))
    {
      ParseChemistryElement(object, curRoot->Find("ChemistryElement"));
    }
    // Get camera information
    if(curRoot->Find("Camera"))
    {
      ParseCamera(object, curRoot->Find("Camera"));
    }
    // Get sound emitter information
    if(curRoot->Find("SoundEmitter"))
    {
      ParseSoundEmitter(object, curRoot->Find("SoundEmitter"));
    }
    // Get sound listener information
    if(curRoot->Find("SoundListener"))
    {
      ParseSoundListener(object, curRoot->Find("SoundListener"));
    }
    // Who is the focus of this level?
    if(curRoot->Find("Focus"))
    {
      bool value = curRoot->Find("Focus")->Find("IsFocus")->GetValue().ToBool();
      if(value)
      {
        mFocusTarget = object;
      }
    }
    if(curRoot->Find("Name"))
    {
      // Accept either the object file or level file version.
      if(curRoot->Find("Name")->Find("Value"))
      {
        object->SetName(curRoot->Find("Name")->Find("Value")->GetValue());
      }
      else
      {
        object->SetName(curRoot->Find("Name")->GetValue());
      }
    }
    if(curRoot->Find("Tags"))
    {
      if(curRoot->Find("Tags")->Find("Clear") && curRoot->Find("Tags")->Find("Clear")->GetValue().ToBool())
        object->ClearTags();
      
      std::vector<HashString> tags = curRoot->Find("Tags")->GetValue().ToHashStringVector();
      for(std::vector<HashString>::iterator it = tags.begin(); it != tags.end(); ++it)
      {
        object->AddTag(*it);
      }
    }
    if(curRoot->Find("Parent"))
    {
      HashString parentName = curRoot->Find("Parent")->GetValue();
      GameObject *parent = FindObject(parentName);
      
      if(!parent)
        assert(!"Parent not found.");
      
      parent->AddChild(object);
      object->SetParent(parent);
    }

    ParserNodeContainer untouched = curRoot->GetUntouchedRoots();
    for(parserNodeIT it = untouched.begin(); it != untouched.end(); ++it)
    {
      ParseAdditionalData(*it, object);
    }

    ++curIndex;
    tempIndex = curObject + Common::IntToString(curIndex);
    curRoot = parser->Find(tempIndex);
  }

  // One or many tile maps, doesn't matter.
  if(parser->Find("TileMapGenerator"))
  {
    ParseTileGenerator(parser->Find("TileMapGenerator"));
  }
  else if(parser->Find("TileMapGenerators"))
  {
    ParserNode* tileMapGenerators = parser->Find("TileMapGenerators");
    int tileMapIndex = 0;
    HashString const TILEMAPGENERATOR = "TileMapGenerator_";
    
    while(tileMapGenerators->Find(TILEMAPGENERATOR + Common::IntToString(tileMapIndex)))
    {
      ParseTileGenerator(tileMapGenerators->Find(TILEMAPGENERATOR + Common::IntToString(tileMapIndex)));
      ++tileMapIndex;
    }
  }
  if(parser->Find("Music"))
  {
    ParserNode *musicNode = parser->Find("Music");
    if(musicNode->Find("Song"))
      mMusicNames.insert(musicNode->Find("Song")->GetValue());
    else
    {
      int index = 0;
      ParserNode *songNode = musicNode->Find("Song_" + Common::IntToString(index));
      if(songNode)
      {
        mMusicNames.insert(songNode->GetValue());
        ++index;
        songNode = musicNode->Find("Song_" + Common::IntToString(index));
      }
    }
  }
  if(parser->Find("Scenario"))
  {
    ParserNode* scenario = parser->Find("Scenario");
    ParseFile(scenario->Find("Name")->GetValue(), scenario->Find("Folder")->GetValue());
    if(scenario->Find("Load")->GetValue().ToBool())
      LoadScenario(scenario->Find("Name")->GetValue());
  }
  if(parser->Find("Bounds"))
  {
    /*
     * Set camera bounds manually
     * 3 places where bounds can be set:
     * 1. Auto set from object transforms, see above
     * 2. Calculated from tilemap, see TileMapGenerator.cpp
     * 3. See below
     */
    int x, y;
    x = parser->Find("Bounds")->Find("MaxX")->GetValue().ToInt();
    y = parser->Find("Bounds")->Find("MaxY")->GetValue().ToInt();
    mMaxBoundary = Vector3(x, y, 0);
    x = parser->Find("Bounds")->Find("MinX")->GetValue().ToInt();
    y = parser->Find("Bounds")->Find("MinY")->GetValue().ToInt();
    mMinBoundary = Vector3(x, y, 0);
  }
  if(parser->Find("ClearColor"))
  {
    float r, g, b, a;
    r = parser->Find("ClearColor")->Find("ColorR")->GetValue().ToFloat();
    g = parser->Find("ClearColor")->Find("ColorG")->GetValue().ToFloat();
    b = parser->Find("ClearColor")->Find("ColorB")->GetValue().ToFloat();
    a = parser->Find("ClearColor")->Find("ColorA")->GetValue().ToFloat();
    mClearColor = Vector4(r, g, b, a);
  }

  ParserNodeContainer untouched = parser->GetBaseRoot()->GetUntouchedRoots();
  for(parserNodeIT it = untouched.begin(); it != untouched.end(); ++it)
  {
    ParseAdditionalData(*it, nullptr);
  }
  
  // Clean up
  delete parser;
}

/**
 * @brief Create and name a blank scenario to edit.
 * @param aScenarioName Name.
 * @return New scenario.
 */
Level::ObjectContainer* Level::CreateEmptyScenario(HashString const &aScenarioName)
{
  if(mScenarios.find(aScenarioName) == mScenarios.end())
  {
    mScenarios[aScenarioName] = ObjectContainer();
    return &mScenarios[aScenarioName];
  }
  else
  {
    DebugLogPrint("%s scenario file already exists", aScenarioName.ToCharArray());
    assert(!"Scenario file already exists.");
  }
}

/**
 * @brief Helper method to do all leg work for scenarios.
 * @param aFileName File name.
 * @param aFolderName Folder where file is located, relative to assets directory.
 * @return ObjectContainer reference.
 */
Level::ObjectContainer* Level::ParseAndLoadScenario(HashString const &aFileName, HashString const &aFolderName)
{
  ParseFile(aFileName, aFolderName);
  LoadScenario(aFileName);
  return &mScenarios[aFileName];
}

/**
 * @brief Load a scenario by file name
 * @param aFileName Name of file to load.
 */
void Level::LoadScenario(HashString const &aFileName)
{
  if(mScenarios.find(aFileName) != mScenarios.end())
  {
    ObjectContainer &objects = mScenarios[aFileName];
    LoadObjects(objects, ObjectPlacement::DEFAULT);
  }
  else
  {
    DebugLogPrint("%s scenario file not found\n", aFileName.ToCharArray());
    assert(!"Scenario file not found.");
  }
}

/**
 * @brief Unload scenario by filename
 * @param aFileName Name of file to unload scenario objects from.
 */
void Level::UnloadScenario(HashString const &aFileName)
{
  if(mScenarios.find(aFileName) != mScenarios.end())
  {
    ObjectContainer &objects = mScenarios[aFileName];
    UnloadObjects(objects);
    for(ObjectIT it = objects.begin(); it != objects.end();)
    {
      if(objects.size() == 0)
        break;
      
      // This method already removes object from scenarios.
      DeleteObjectDelayed(*it);
      it = objects.begin();
    }
    objects.clear();
    mScenarios.erase(aFileName);
  }
  else
  {
    DebugLogPrint("%s scenario file not found\n", aFileName.ToCharArray());
    assert(!"Scenario file not found.");
  }
}

/**
 * @brief Retrieve scenario
 * @param aFileName Name of scenario
 * @return Scenario if found.
 */
Level::ObjectContainer* Level::GetScenario(HashString const &aFileName)
{
  if(mScenarios.find(aFileName) != mScenarios.end())
  {
    return &mScenarios.at(aFileName);
  }
  else
  {
    DebugLogPrint("%s scenario file not found\n", aFileName.ToCharArray());
    return nullptr;
  }
}

/**
 * @brief Unload all scenarios
 */
void Level::UnloadScenarios()
{
  for(FileContainerIT it = mScenarios.begin(); it != mScenarios.end();)
  {
    if(mScenarios.size() == 1)
      break;
    if(it->first == mFileName)
    {
      ++it;
      continue;
    }
    UnloadScenario(it->first);
    it = mScenarios.begin();
  }
}

/**
 * @brief Helper function to parse self
 */
void Level::ParseBaseFile()
{
  ParseFile(mFileName, mFolderName);
}

/**
 * @brief Get all objects in level.
 * @return All objects.
 */
Level::ObjectContainer& Level::GetObjects(ObjectPlacement const aPlacement)
{
  return mObjects[aPlacement];
}

/**
 * @brief Find object in container.
 * @param aContainer Container to find object in.
 * @param aName Name of object.
 * @return Object, or nullptr if nothing found.
 */
GameObject* Level::FindObject(ObjectContainer const &aContainer, HashString const &aName)
{
  // This is designed to be naive
  // if two objects share the same name...
  // well, too bad.
  ConstObjectIT end = aContainer.end();
  for(ConstObjectIT it = aContainer.begin(); it != end; ++it)
  {
    if(aName == (*it)->GetName() || aName == (*it)->GetFileName())
      return *it;
  }
  return nullptr;
}

/**
 * @brief Remove object's children
 * @param aObject
 */
void Level::DeleteObjectChildren(GameObject *aObject)
{
  GameObject::GameObjectContainer children = aObject->GetChildren();
  for(GameObject::GameObjectIT it = children.begin(); it != children.end(); ++it)
  {
    DeleteObject(it->second);
  }
}

/**
 * @brief Remove object's children
 * @param aObject
 */
void Level::DeleteObjectChildrenDelayed(GameObject *aObject)
{
  GameObject::GameObjectContainer children = aObject->GetChildren();
  for(GameObject::GameObjectIT it = children.begin(); it != children.end(); ++it)
  {
    DeleteObjectDelayed(it->second);
  }
}

/**
 * @brief Serialize out objects
 * @param aParser Parser to write objects to
 * @param aObjects Objects to write
 */
void Level::SerializeObjects(Parser &aParser, ObjectContainer &aObjects)
{
  int curIndex = 0;
  HashString object = "Object_";
  for(ObjectIT it = aObjects.begin(); it != aObjects.end(); ++it)
  {
    HashString objectString = object + Common::IntToString(curIndex);
    aParser.SetCurrentObjectIndex(curIndex);
    aParser.Place(objectString, "");
    
    ParserNode *objectNode = aParser.Find(objectString);
    (*it)->Serialize(objectNode);

    if(*it == mFocusTarget)
    {
      objectNode->Place("Focus", "");
      objectNode->Find("Focus")->Place("IsFocus", "true");
    }
    ++curIndex;
  }
}

/**
 * @brief Serialize all scenario files into separate chunks.
 * @param aParser Parser to get filename and directory from. (To be in same place as main level file.)
 */
void Level::SerializeScenarios(Parser &aParser)
{
  int curIndex = 0;
  HashString parserFile = aParser.GetFilename();
  std::vector<HashString> splitParserFile = parserFile.Split("/");
  HashString fileName = splitParserFile[splitParserFile.size() - 1].Split(".")[0] + "Scenario";
  HashString folder = splitParserFile[splitParserFile.size() - 2];
  for(FileContainerIT it = mScenarios.begin(); it != mScenarios.end(); ++it)
  {
    if(it->first == mFileName)
    {
      continue;
    }
    HashString currentFileName = fileName + Common::IntToString(curIndex);
    Parser *parser = ParserFactory::CreateOutputParser(folder, currentFileName + ".txt");
    SerializeObjects(*parser, it->second);
    parser->Write();
    delete parser;
    ++curIndex;
  }
}

/**
 * @brief Helper to find lingering objects.
 * @param aObject Object to search for.
 * @return Whether file is in a scenario or not.
 */
bool Level::ObjectNotInScenario(GameObject *aObject)
{
  for(FileContainerIT it = mScenarios.begin(); it != mScenarios.end(); ++it)
  {
    for(ObjectIT it2 = it->second.begin(); it2 != it->second.end(); ++it2)
    {
      if(aObject == *it2)
        return false;
    }
  }
  return true;
}

/**
 * @brief Remove object from all scenarios.
 * @param aObject Object to remove.
 */
void Level::RemoveObjectFromScenarios(GameObject *aObject)
{
  for(FileContainerIT it = mScenarios.begin(); it != mScenarios.end(); ++it)
  {
    for(ObjectIT it2 = it->second.begin(); it2 != it->second.end(); ++it2)
    {
      if(aObject == *it2)
      {
        it->second.erase(it2);
        break;
      }
    }
  }
}

/**
 * @brief Get transform data for an object from a root.
 */
void Level::ParseTransform(GameObject *aObject, ParserNode *aTransform)
{
  Transform* objTransform = aObject->GET<Transform>();
  objTransform->Deserialize(aTransform);
  
  Vector3 pos = objTransform->GetPosition();
  Vector3 size = objTransform->GetSize();

  // Auto set camera bounds based on objects in environment
  mMinBoundary.x = Lesser<float>(pos.x - size.x, mMinBoundary.x);
  mMinBoundary.y = Lesser<float>(pos.y - size.y, mMinBoundary.y);
  mMaxBoundary.x = Greater<float>(pos.x + size.x, mMaxBoundary.x);
  mMaxBoundary.y = Greater<float>(pos.y + size.y, mMaxBoundary.y);
}

/**
 * @brief Get surface data from a root.
 */
void Level::ParseSurface(GameObject *aObject, ParserNode *aSurface)
{
  Surface* objSurface = aObject->GET<Surface>();
  objSurface->Deserialize(aSurface);
}

/**
 * @brief Get camera data from a root.
 */
void Level::ParseCamera(GameObject *aObject, ParserNode* aCamera)
{
  Camera *camera = aObject->GET<Camera>();
  if(camera == nullptr)
  {
    camera = GetManager()->GetOwningApp()->GET<GraphicsManager>()->CreateCamera();
    aObject->AddComponent(camera);
  }
  camera->Deserialize(aCamera);
}

/**
 * @brief Get text data from root.
 */
void Level::ParseText(GameObject *aObject, ParserNode* aText)
{
  Surface* objText = aObject->GET<Surface>();
  objText->Deserialize(aText);
}

/**
 * @brief Get physics object data from a root.
 */
void Level::ParsePhysicsObject(GameObject *aObject, ParserNode* aPhysicsObject)
{
  // If object doesn't have physicsobject, it does now.
  PhysicsObject* physicsObject = aObject->GET<PhysicsObject>();
  if(!physicsObject)
  {
    physicsObject = new PhysicsObject(GetManager()->GetOwningApp()->GET<PhysicsWorld>());
    aObject->AddComponent(physicsObject);
  }
  
  physicsObject->Deserialize(aPhysicsObject);
}

/**
 * @brief Get chemistry material data from root.
 */
void Level::ParseChemistryMaterial(GameObject *aObject, ParserNode* aChemistryMaterial)
{
  // If object doesn't have chemistryMaterial, it does now.
  ChemistryMaterial* chemistryMaterial = aObject->GET<ChemistryMaterial>();
  if(!chemistryMaterial)
  {
    chemistryMaterial = GetManager()->GetOwningApp()->GET<ChemistryManager>()->CreateMaterial(aChemistryMaterial->Find("Name")->GetValue());
    aObject->AddComponent(chemistryMaterial);
  }
  
  chemistryMaterial->Deserialize(aChemistryMaterial);
}

/**
 * @brief Get chemistry element data from root.
 */
void Level::ParseChemistryElement(GameObject *aObject, ParserNode* aChemistryElement)
{
  // If object doesn't have chemistryElement, it does now.
  ChemistryElement* chemistryElement = aObject->GET<ChemistryElement>();
  if(!chemistryElement)
  {
    chemistryElement = GetManager()->GetOwningApp()->GET<ChemistryManager>()->CreateElement(aChemistryElement->Find("Name")->GetValue());
    aObject->AddComponent(chemistryElement);
  }
  
  chemistryElement->Deserialize(aChemistryElement);
}

/**
 * @brief Get follow data from root.
 */
void Level::ParseFollowComponent(GameObject *aObject, ParserNode* aFollowComponent)
{
  FollowComponent *followComponent = aObject->GET<FollowComponent>();
  if(!followComponent)
  {
    followComponent = new FollowComponent();
    aObject->AddComponent(followComponent);
  }
  
  followComponent->Deserialize(aFollowComponent);
}

/**
 * @brief Get sound emitter data from root.
 */
void Level::ParseSoundEmitter(GameObject *aObject, ParserNode* aSoundEmitter)
{
  SoundEmitter *soundEmitter = aObject->GET<SoundEmitter>();
  if(!soundEmitter)
  {
    soundEmitter = new SoundEmitter();
    aObject->AddComponent(soundEmitter);
  }
  
  soundEmitter->Deserialize(aSoundEmitter);
}

/**
 * @brief Get sound listener data from root.
 */
void Level::ParseSoundListener(GameObject *aObject, ParserNode* aSoundListener)
{
  SoundListener *soundListener = aObject->GET<SoundListener>();
  if(!soundListener)
  {
    soundListener = new SoundListener();
    aObject->AddComponent(soundListener);
  }
  
  soundListener->Deserialize(aSoundListener);
}

/**
 * @brief Parse effects root.
 */
void Level::ParseEffects(GameObject *aObject, ParserNode *aEffects)
{
  EffectsManager* effectsManager = GetManager()->GetOwningApp()->GET<EffectsManager>();
  int curIndex = 0;
  HashString const effectString = "Effect_";
  HashString curEffect = effectString + Common::IntToString(curIndex);
  while(aEffects->Find(curEffect))
  {
    ParserNode* effectRoot = aEffects->Find(curEffect);
    HashString type = effectRoot->Find("Type")->GetValue();
    HashString name = effectRoot->Find("Name")->GetValue();
    float time = effectRoot->Find("Time")->GetValue().ToFloat();
    Effect* effect = effectsManager->CreateEffect(type);
    effect->SetName(name);
    effect->SetObject(aObject);
    effect->SetTime(time);
    effect->Deserialize(effectRoot);
    ++curIndex;
    curEffect = effectString + Common::IntToString(curIndex);
  }
}

/**
 * @brief Parse custom script root.
 */
void Level::ParseCustomScript(GameObject *aObject, ParserNode *aCustomScript)
{
  CustomScript *customScript = aObject->GET<CustomScript>();
  if(!customScript)
  {
    customScript = new CustomScript();
    aObject->AddComponent(customScript);
  }
  
  customScript->Deserialize(aCustomScript);
}

/**
 * @brief Create tile generator helper.
 * @param aTileMap
 */
void Level::ParseTileGenerator(ParserNode *aTileMap)
{
  HashString value, empty;
  int width, height, tileSize;
  HashString file, frameDataFilename, settingsDataFileName;
  std::vector<int> frames, collision, shapes, materials;
  std::unordered_map<int, float> heights;
  std::unordered_map<int, std::vector<int>> animations;
  std::unordered_map<int, HashString> materialInfo;
  float tileAnimationSpeed = 0.0f;
  float zOffset = 0;

  width = aTileMap->Find("Width")->GetValue().ToInt();
  height = aTileMap->Find("Height")->GetValue().ToInt();
  tileSize = aTileMap->Find("TileSize")->GetValue().ToInt();
  file = aTileMap->Find("Image")->GetValue();
  frameDataFilename = aTileMap->Find("Data")->GetValue();
  
  // Reserve the tilemap data to avoid reallocs.
  int totalAllocation = width * height;
  frames.reserve(totalAllocation);
  collision.reserve(totalAllocation);
  shapes.reserve(totalAllocation);
  
  // If there's ZOffset data, use it.
  if(aTileMap->Find("ZOffset"))
  {
    zOffset = aTileMap->Find("ZOffset")->GetValue().ToFloat();
  }
  
  // If there's setting data, parse it in and use it.
  if(aTileMap->Find("SettingsFile"))
  {
    HashString const height = "Height_";
    HashString const animation = "Animation_";
    HashString const material = "Material_";
    settingsDataFileName = aTileMap->Find("SettingsFile")->GetValue();
    Parser *settingsData = ParserFactory::CreateInputParser("Maps", settingsDataFileName);
    
    // Heightmap
    int index = 0;
    HashString curIndex = height + Common::IntToString(index);
    while(settingsData->Find(curIndex))
    {
      std::vector<float> values = Common::StringToFloatVector(settingsData->Find(curIndex)->GetValue());
      heights[static_cast<int>(values[0])] = values[1];
      ++index;
      curIndex = height + Common::IntToString(index);
    }

    // Animations
    index = 0;
    curIndex = animation + Common::IntToString(index);
    while(settingsData->Find(curIndex))
    {
      int tileID = settingsData->Find(curIndex)->Find("TileID")->GetValue().ToInt();
      std::vector<int> animationData = settingsData->Find(curIndex)->Find("Animation")->GetValue().ToIntVector();
      animations[tileID] = animationData;
      ++index;
      curIndex = animation + Common::IntToString(index);
    }
    
    // Materials
    index = 0;
    curIndex = material + Common::IntToString(index);
    while(settingsData->Find(curIndex))
    {
      ParserNode* base = settingsData->Find(curIndex);
      HashString name = base->Find("Name")->GetValue();
      materialInfo[index] = name;
      ++index;
      curIndex = material + Common::IntToString(index);
    }

    // Animation speed
    if(settingsData->Find("AnimationSpeed"))
    {
      tileAnimationSpeed = settingsData->Find("AnimationSpeed")->GetValue().ToFloat();
    }
    
    delete settingsData;
  }

  Parser *tileMapData = ParserFactory::CreateInputParser("Maps", frameDataFilename);
  frames = Common::StringToIntVector(tileMapData->Find("MapArtData")->GetValue());
  collision = Common::StringToIntVector(tileMapData->Find("Collision")->GetValue());
  
  if(tileMapData->Find("CollisionShapes"))
  {
    shapes = Common::StringToIntVector(tileMapData->Find("CollisionShapes")->GetValue());
  }
  if(tileMapData->Find("Materials"))
  {
    materials = Common::StringToIntVector(tileMapData->Find("Materials")->GetValue());
  }
  delete tileMapData;

  mGenerators.push_back(new TileMapGenerator(width, height, tileSize,
                                   zOffset, file, frameDataFilename,
                                   frames, collision, shapes,
                                   materials, heights, materialInfo,
                                   animations, tileAnimationSpeed, this));
}
