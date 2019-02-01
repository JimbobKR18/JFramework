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
#include "ComponentFactory.h"

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
             mName(""), mFolderName(aFolderName), mFileName(aFileName), mSoundNames(), mObjects(),
             mOwner(aManager), mGenerators(), mFocusTarget(nullptr), mClearColor(0, 0, 0, 1), mSoundBanks(), 
             mSoundChannels(), mMaxBoundary(0,0,0), mMinBoundary(0,0,0), mScenarios()
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
  mSoundNames.clear();
  mSoundChannels.clear();
  mSoundBanks.clear();
  mScenarios.clear();
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
* @brief Get the sound names.
* @return The Sound names.
*/
Level::SoundNameContainer Level::GetSoundNames() const
{
  return mSoundNames;
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
 * @brief Get sound channels
 * @return Sound channels
 */
Level::SoundChannelContainer Level::GetSoundChannels() const
{
  return mSoundChannels;
}

/**
 * @brief Get sound banks
 * @return Sound banks
 */
Level::SoundBankContainer Level::GetSoundBanks() const
{
  return mSoundBanks;
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
 * @brief Find all objects with name.
 * @param aObjectName
 * @return All objects whose file name or object name match input.
 */
Level::ObjectVector Level::FindObjects(HashString const &aName)
{
  ObjectVector ret;
  for(int i = ObjectPlacement::DEFAULT; i != ObjectPlacement::PLACEMENT_ALL; ++i)
  {
    ObjectIT end = mObjects[i].end();
    for(ObjectIT it = mObjects[i].begin(); it != end; ++it)
    {
      if(aName == (*it)->GetName() || aName == (*it)->GetFileName())
        ret.push_back(*it);
    }
  }
  return ret;
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
  aObject->SetPlacement(aPlacement);
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
        effectsManager->RemoveEffectsForObject(aObject);
        objectManager->DeleteObject(aObject);
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
    if(mObjects[i].find(aObject) != mObjects[i].end())
    {
      RemoveObjectFromScenarios(aObject);
      mObjects[i].erase(aObject);
      DeleteObjectChildrenDelayed(aObject);
      
      ObjectDeleteMessage *msg = new ObjectDeleteMessage(aObject);
      objectManager->ProcessDelayedMessage(msg);
      effectsManager->RemoveEffectsForObject(aObject);
    }
  }
}

/**
 * @brief Delete all objects in level.
 */
void Level::DeleteObjects()
{
  ObjectManager *objectManager = mOwner->GetOwningApp()->GET<ObjectManager>();
  EffectsManager *effectsManager = mOwner->GetOwningApp()->GET<EffectsManager>();
  for(int i = ObjectPlacement::DEFAULT; i != ObjectPlacement::PLACEMENT_ALL; ++i)
  {
    ObjectIT end = mObjects[i].end();
    for(ObjectIT it = mObjects[i].begin(); it != end; ++it)
    {
      RemoveObjectFromScenarios(*it);
      effectsManager->RemoveEffectsForObject(*it);
      objectManager->DeleteObject(*it);
    }
    mObjects[i].clear();
  }
  mScenarios.clear();
  objectManager->ClearMessages();
  objectManager->GetOwningApp()->ClearDelayedMessages();
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
  graphicsManager->ClearTree();
  
  // Delete tile maps.
  for(TileMapGeneratorContainerIT it = mGenerators.begin(); it != mGenerators.end(); ++it)
    delete *it;
  mGenerators.clear();
  mSoundNames.clear();
  
  // Delete objects and reload.
  DeleteObjects();
  ParseBaseFile();
  PostReset();
}

/**
 * @brief Evaluate size at location and object level bounds.
 * @param aPosition Center
 * @param aSize Size of point
 */
void Level::UpdateBoundaries(Vector3 const &aPosition, Vector3 const &aSize)
{
  mMinBoundary.x = Lesser<float>(aPosition.x - aSize.x, mMinBoundary.x);
  mMinBoundary.y = Lesser<float>(aPosition.y - aSize.y, mMinBoundary.y);
  mMaxBoundary.x = Greater<float>(aPosition.x + aSize.x, mMaxBoundary.x);
  mMaxBoundary.y = Greater<float>(aPosition.y + aSize.y, mMaxBoundary.y);
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
void Level::Load(Level *aPrevLevel)
{
  // Load all objects
  for(int i = ObjectPlacement::DEFAULT; i != ObjectPlacement::PLACEMENT_ALL; ++i)
    LoadObjects(mObjects[i], static_cast<ObjectPlacement>(i));

  LoadSounds(aPrevLevel);

  mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->SetClearColor(mClearColor);
  mOwner->GetOwningApp()->GET<GraphicsManager>()->SetUnsortedLayers(mUnsortedLayers);
  mOwner->GetOwningApp()->GET<GraphicsManager>()->ResizeTree(mMinBoundary, mMaxBoundary);
  
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
    
  UnloadSounds(aNextLevel);

  GraphicsManager *graphicsManager = mOwner->GetOwningApp()->GET<GraphicsManager>();
  graphicsManager->SetPrimaryCamera(nullptr);
  graphicsManager->ClearTree();
}

/**
 * @brief Load all objects in list into view.
 * @param aObjects Objects to add.
 */
void Level::LoadObjects(ObjectContainer const &aObjects, ObjectPlacement const aPlacement)
{
  for(ConstObjectIT it = aObjects.begin(); it != aObjects.end(); ++it)
  {
    LoadObject(*it, aPlacement);
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
    UnloadObject(*it);
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
  
  // Sound
  index = 0;
  aParser.Place("Sound", "");
  for(SoundNameContainerIT it = mSoundNames.begin(); it != mSoundNames.end(); ++it, ++index)
  {
    aParser.Place("Sound", "SoundName_" + Common::IntToString(index), it->first);
    aParser.Place("Sound", "Group_" + Common::IntToString(index), it->second);
  }
  index = 0;
  for(SoundBankContainerIT it = mSoundBanks.begin(); it != mSoundBanks.end(); ++it, ++index)
  {
    aParser.Place("Sound", "Bank_" + Common::IntToString(index), *it);
  }
  
  // Clear color
  aParser.Place("ClearColor", "");
  aParser.Place("ClearColor", "ColorR", Common::FloatToString(mClearColor.x));
  aParser.Place("ClearColor", "ColorG", Common::FloatToString(mClearColor.y));
  aParser.Place("ClearColor", "ColorB", Common::FloatToString(mClearColor.z));
  aParser.Place("ClearColor", "ColorA", Common::FloatToString(mClearColor.w));
  
  // Unsorted layers
  std::set<int> tempLayers;
  for(std::unordered_set<int>::const_iterator it = mUnsortedLayers.begin(); it != mUnsortedLayers.end(); ++it)
  {
    tempLayers.insert(*it);
  }
  aParser.Place("Layers", "");
  aParser.Place("Layers", "Unsorted", Common::IntSetToString(tempLayers));
  
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
 * @brief Get level type
 * @return Type of level
 */
HashString Level::GetType() const
{
  return "Level";
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
    
    // Create components from factory.
    ComponentFactory *factory = mOwner->GetOwningApp()->GetComponentFactory();
    ParserNodeContainer children = curRoot->GetChildren();
    for(parserNodeIT it = children.begin(); it != children.end(); ++it)
    {
      factory->CreateComponent(mOwner->GetOwningApp(), object, *it);
    }

    // Get transform information and adjust bounds.
    ParseTransform(object);
    
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
  if(parser->Find("Sound") || parser->Find("Music"))
  {
    HashString const defaultGroupName = "Music";
    ParserNode *soundNode = parser->Find("Sound");
    if(!soundNode)
      soundNode = parser->Find("Music");
    
    int index = 0;
    ParserNode *bankNode = soundNode->Find("Bank_" + Common::IntToString(index));
    while(bankNode)
    {
      HashString bankName = bankNode->GetValue();
          
      mSoundBanks.insert(bankName);
      ++index;
      bankNode = soundNode->Find("Bank_" + Common::IntToString(index));
    }
    
    if(soundNode->Find("SoundName"))
    {
      HashString soundName = soundNode->Find("SoundName")->GetValue();
      HashString group = defaultGroupName;
      if(soundNode->Find("Group"))
        group = soundNode->Find("Group")->GetValue();
      mSoundNames[soundName] = group;
    }
    // Compatibility layer.
    else if(soundNode->Find("Song"))
    {
      HashString soundName = soundNode->Find("Song")->GetValue();
      HashString group = defaultGroupName;
      if(soundNode->Find("Group"))
        group = soundNode->Find("Group")->GetValue();
      mSoundNames[soundName] = group;
    }
    else
    {
      int index = 0;
      ParserNode *songNode = soundNode->Find("SoundName_" + Common::IntToString(index));
      ParserNode *groupNode = soundNode->Find("Group_" + Common::IntToString(index));
      if(songNode)
      {
        HashString soundName = songNode->GetValue();
        HashString group = defaultGroupName;
        if(groupNode)
          group = groupNode->GetValue();
          
        mSoundNames[soundName] = group;
        ++index;
        songNode = soundNode->Find("SoundName_" + Common::IntToString(index));
        songNode = soundNode->Find("Group_" + Common::IntToString(index));
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
  if(parser->Find("Layers"))
  {
    ParserNode *layers = parser->Find("Layers");
    if(layers->Find("Unsorted"))
    {
      std::set<int> tempLayers = layers->Find("Unsorted")->GetValue().ToIntSet();
      for(std::set<int>::const_iterator it = tempLayers.begin(); it != tempLayers.end(); ++it)
      {
        mUnsortedLayers.insert(*it);
      }
    }
  }

  ParserNodeContainer untouched = parser->GetBaseRoot()->GetUntouchedRoots();
  for(parserNodeIT it = untouched.begin(); it != untouched.end(); ++it)
  {
    ParseAdditionalData(*it, nullptr);
  }
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
 * @brief Load sounds for level
 * @param aPrevLevel Previous level
 */
void Level::LoadSounds(Level *aPrevLevel)
{
  SoundManager *soundManager = mOwner->GetOwningApp()->GET<SoundManager>();
  for(SoundBankContainerIT it = mSoundBanks.begin(); it != mSoundBanks.end(); ++it)
  {
    if(!aPrevLevel || aPrevLevel->mSoundBanks.find(*it) == aPrevLevel->mSoundBanks.end())
    {
      soundManager->LoadSoundBank(*it);
    }
  }
  for(SoundNameContainerIT it = mSoundNames.begin(); it != mSoundNames.end(); ++it)
  {
    if(!aPrevLevel || aPrevLevel->mSoundNames.find(it->first) == aPrevLevel->mSoundNames.end())
    {
      soundManager->CreateChannelGroup(it->second);
      int channel = soundManager->PlaySound(it->first, SoundManager::INFINITE_LOOPS);
      mSoundChannels[channel] = it->first;
      soundManager->AddChannelToGroup(it->second, channel);
    }
  }
}

/**
 * @brief Unload sounds for level
 * @param aNextLevel Next level
 */
void Level::UnloadSounds(Level *aNextLevel)
{
  SoundManager *soundManager  = mOwner->GetOwningApp()->GET<SoundManager>();
  for(SoundBankContainerIT it = mSoundBanks.begin(); it != mSoundBanks.end(); ++it)
  {
    if(!aNextLevel || aNextLevel->mSoundBanks.find(*it) == aNextLevel->mSoundBanks.end())
    {
      soundManager->UnloadSoundBank(*it);
    }
  }
  for(SoundChannelContainerIT it = mSoundChannels.begin(); it != mSoundChannels.end(); ++it)
  {
    if(!aNextLevel || aNextLevel->mSoundNames.find(it->second) == aNextLevel->mSoundNames.end())
    {
      soundManager->RemoveChannelFromGroups(it->first);
      soundManager->StopChannel(it->first);
    }
    else if(aNextLevel)
    {
      aNextLevel->mSoundChannels[it->first] = it->second;
      soundManager->AddChannelToGroup(aNextLevel->mSoundNames[it->second], it->first);
    }
  }
  if(this != aNextLevel)
    mSoundChannels.clear();
}

/**
 * @brief Load object's components in
 * @param aObject Object to load
 */
void Level::LoadObject(GameObject *aObject, ObjectPlacement const aPlacement)
{
  aObject->SetPlacement(aPlacement);
  mOwner->GetOwningApp()->GET<ObjectManager>()->AddObject(aObject, (aPlacement == ObjectPlacement::STATIC) ? true : false);
  if(aObject->GET<PhysicsObject>())
    mOwner->GetOwningApp()->GET<PhysicsWorld>()->AddObject(aObject->GET<PhysicsObject>());
  if(aObject->GET<Surface>())
  {
    Surface *surface = aObject->GET<Surface>();
    if(surface->GetUIElement())
      mOwner->GetOwningApp()->GET<GraphicsManager>()->AddUISurface(aObject->GET<Surface>());
    else
      mOwner->GetOwningApp()->GET<GraphicsManager>()->AddSurface(aObject->GET<Surface>());
  }
  if(aObject->GET<Controller>())
    mOwner->GetOwningApp()->GET<ControllerManager>()->AddController(aObject->GET<Controller>());
  if(aObject->GET<ChemistryMaterial>())
    mOwner->GetOwningApp()->GET<ChemistryManager>()->AddMaterial(aObject->GET<ChemistryMaterial>());
  if(aObject->GET<ChemistryElement>())
    mOwner->GetOwningApp()->GET<ChemistryManager>()->AddElement(aObject->GET<ChemistryElement>());
  if(aObject->GET<Camera>())
    mOwner->GetOwningApp()->GET<GraphicsManager>()->AddCamera(aObject->GET<Camera>());
  if(aObject->GET<Camera>() && aObject->GET<Camera>()->GetPrimary())
    mOwner->GetOwningApp()->GET<GraphicsManager>()->SetPrimaryCamera(aObject->GET<Camera>());
  if(aObject->GET<FollowComponent>())
    aObject->GET<FollowComponent>()->ResetTarget();
}

/**
 * @brief Unload object's components
 * @param aObject Object to unload
 */
void Level::UnloadObject(GameObject *aObject)
{
  mOwner->GetOwningApp()->GET<ObjectManager>()->RemoveObject(aObject);
  if(aObject->GET<PhysicsObject>())
    mOwner->GetOwningApp()->GET<PhysicsWorld>()->RemoveObject(aObject->GET<PhysicsObject>());
  if(aObject->GET<Surface>())
    mOwner->GetOwningApp()->GET<GraphicsManager>()->RemoveSurface(aObject->GET<Surface>());
  if(aObject->GET<Controller>())
    mOwner->GetOwningApp()->GET<ControllerManager>()->RemoveController(aObject->GET<Controller>());
  if(aObject->GET<ChemistryMaterial>())
    mOwner->GetOwningApp()->GET<ChemistryManager>()->RemoveMaterial(aObject->GET<ChemistryMaterial>());
  if(aObject->GET<ChemistryElement>())
    mOwner->GetOwningApp()->GET<ChemistryManager>()->RemoveElement(aObject->GET<ChemistryElement>());
  if(aObject->GET<Camera>())
    mOwner->GetOwningApp()->GET<GraphicsManager>()->RemoveCamera(aObject->GET<Camera>());
  if(aObject->GET<SoundEmitter>())
    aObject->GET<SoundEmitter>()->StopSound();
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
    if(it->second.find(aObject) != it->second.end())
      return false;
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
    it->second.erase(aObject);
  }
}

/**
 * @brief Get transform data for an object from a root.
 */
void Level::ParseTransform(GameObject *aObject)
{
  Transform* objTransform = aObject->GET<Transform>();
  Vector3 pos = objTransform->GetPosition();
  Vector3 size = objTransform->GetSize();

  // Auto set camera bounds based on objects in environment
  UpdateBoundaries(pos, size);
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
  Vector3 collisionOffset;
  std::vector<int> frames, collision, shapes, materials;
  std::unordered_map<int, float> heights;
  std::unordered_map<int, std::vector<int>> animations;
  std::unordered_map<int, HashString> materialInfo;
  std::unordered_map<int, bool> emptyTiles;
  float tileAnimationSpeed = 0.0f;
  float zOffset = 0;
  int layer = 0;

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
  
  // If there's offset data, use it.
  if(aTileMap->Find("ZOffset"))
  {
    zOffset = aTileMap->Find("ZOffset")->GetValue().ToFloat();
  }
  if(aTileMap->Find("CollisionOffset"))
  {
    collisionOffset = aTileMap->Find("CollisionOffset")->GetValue().ToVector3();
  }
  if(aTileMap->Find("Layer"))
  {
    layer = aTileMap->Find("Layer")->GetValue().ToInt();
  }
  
  // If there's setting data, parse it in and use it.
  if(aTileMap->Find("SettingsFile"))
  {
    HashString const height = "Height_";
    HashString const animation = "Animation_";
    HashString const material = "Material_";
    HashString const empty = "EmptyTile_";
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
    
    // Empty Tiles
    index = 0;
    curIndex = empty + Common::IntToString(index);
    while(settingsData->Find(curIndex))
    {
      ParserNode* base = settingsData->Find(curIndex);
      int emptyTileIndex = base->Find("TileID")->GetValue().ToInt();
      bool isEmpty = base->Find("Empty")->GetValue().ToBool();
      emptyTiles[emptyTileIndex] = isEmpty;
      ++index;
      curIndex = empty + Common::IntToString(index);
    }

    // Animation speed
    if(settingsData->Find("AnimationSpeed"))
    {
      tileAnimationSpeed = settingsData->Find("AnimationSpeed")->GetValue().ToFloat();
    }
  }

  Parser *tileMapData = ParserFactory::CreateInputParser("Maps", frameDataFilename);
  frames = tileMapData->Find("MapArtData")->GetValue().ToIntVector();
  collision = tileMapData->Find("Collision")->GetValue().ToIntVector();
  
  if(tileMapData->Find("CollisionShapes"))
  {
    shapes = tileMapData->Find("CollisionShapes")->GetValue().ToIntVector();
  }
  if(tileMapData->Find("Materials"))
  {
    materials = tileMapData->Find("Materials")->GetValue().ToIntVector();
  }

  mGenerators.push_back(new TileMapGenerator(width, height, tileSize, layer,
                                   zOffset, file, frameDataFilename, collisionOffset,
                                   frames, collision, shapes,
                                   materials, heights, materialInfo,
                                   emptyTiles, animations, tileAnimationSpeed,
                                   this));
}
