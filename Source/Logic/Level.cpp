#include "Level.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "MathExt.h"
#include "Transform.h"
#include "ChemistryMaterial.h"
#include "ChemistryElement.h"
#include "GraphicsManager.h"
#include "ChemistryManager.h"
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
#include "Menu.h"

#if !defined(__APPLE__) && !defined(IOS) && !defined(ANDROID)
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
             mName(""), mFolderName(aFolderName), mFileName(aFileName), mMusicName(""), mObjects(),
             mMenus(), mOwner(aManager), mGenerator(nullptr),
             mFocusTarget(nullptr), mClearColor(0, 0, 0, 1), mMaxBoundary(0,0,0), 
             mMinBoundary(0,0,0), mScenarios()
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
  if(mGenerator)
    delete mGenerator;
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
* @brief Get the music name.
* @return The music name.
*/
HashString Level::GetMusicName() const
{
  return mMusicName;
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
 * @brief Get the tile map generator. You can get individual tiles and other helperful things here.
 * @return The tile map generator.
 */
TileMapGenerator* Level::GetTileMap() const
{
  return mGenerator;
}

/**
 * @brief Get the camera's focus target for the level.
 * @return The camera target.
 */
GameObject* Level::GetFocusTarget() const
{
  return mFocusTarget;
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
      if(aObjectName == (*it)->GetName())
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
    Cube cube(transform->GetPosition(), transform->GetSize());

    if(cube.Get2DCollision(aPosition))
    {
      ret.push_back(*it);
    }
  }
  std::sort(ret.begin(), ret.end(), SortPredicate);
  return ret;
}

/**
 * @brief Find a menu by name.
 * @param aMenuName
 * @return The menu, or nullptr.
 */
Menu* Level::FindMenu(HashString const &aMenuName)
{
  for(ConstMenuIT it = mMenus.begin(); it != mMenus.end(); ++it)
  {
    if((*it)->GetName() == aMenuName)
    {
      return *it;
    }
  }
  return nullptr;
}

/**
 * @brief Add a menu to our level.
 * @param aMenu
 */
void Level::AddMenu(Menu *aMenu)
{
  if(!FindMenu(aMenu->GetName()))
    mMenus.push_back(aMenu);
}

/**
 * @brief Remove a menu from our level.
 * @param aMenu
 */
void Level::RemoveMenu(Menu *aMenu)
{
  if(aMenu == nullptr)
    return;

  for(MenuIT it = mMenus.begin(); it != mMenus.end(); ++it)
  {
    if(*it == aMenu)
    {
      delete *it;
      mMenus.erase(it);
      return;
    }
  }
  
  assert(!"Menu not found, you sure you added it to this list?");
}

/**
 * @brief Remove all menus, the right way.
 */
void Level::RemoveMenus()
{
  for(MenuIT it = mMenus.begin(); it != mMenus.end(); ++it)
  {
    delete *it;
  }
  mMenus.clear();
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
  GraphicsManager *graphicsManager = mOwner->GetOwningApp()->GET<GraphicsManager>();
  
  // Unassociate object from view target if need be
  if(graphicsManager->GetScreen()->GetView().GetTarget() == aObject)
  {
    graphicsManager->GetScreen()->GetView().SetTarget(nullptr);
  }
  
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
        break;
      }
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
  for(MenuIT it = mMenus.begin(); it != mMenus.end(); ++it)
  {
    delete *it;
  }
  mMenus.clear();
  manager->ClearObjects();
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
  graphicsManager->GetScreen()->GetView().SetTarget(nullptr);
  // NOTE: Removes menus too
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

  if(!mMusicName.Empty() && (!aPrevLevel || aPrevLevel->mMusicName != mMusicName))
    mOwner->GetOwningApp()->GET<SoundManager>()->PlaySound(mMusicName, Sound::INFINITE_LOOPS);

  mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(mFocusTarget);

  mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->SetClearColor(mClearColor);
  mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetMaxBoundary(mMaxBoundary);
  mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetMinBoundary(mMinBoundary);
}

/**
 * @brief Specify all objects to be be displayed / updated.
 * @param aNextLevel
 */
void Level::Unload(Level const *aNextLevel)
{
  // Unload all objects
  for(int i = ObjectPlacement::DEFAULT; i != ObjectPlacement::PLACEMENT_ALL; ++i)
    UnloadObjects(mObjects[i]);
    
  // Remove menus because they are not level files.
  RemoveMenus();

  if(!mMusicName.Empty() && (!aNextLevel || aNextLevel->mMusicName != mMusicName))
    mOwner->GetOwningApp()->GET<SoundManager>()->StopSound(mMusicName);

  mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(nullptr);
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
  
  for(MenuIT it = mMenus.begin(); it != mMenus.end(); ++it)
  {
    (*it)->Update();
  }

  // Update tile animations if possible
  if(mGenerator)
  {
    mGenerator->Update();
  }
}

/**
 * @brief Any extra nodes inside of a level file need to be parsed, but if we reach here then you are autoparsing a level file that derives from level.
 * @param aRoot Root node to read from.
 * @param aObject Object to apply data to.
 */
void Level::ParseAdditionalData(Root *aRoot, GameObject *aObject)
{ 
  aObject->ParseAdditionalData(aRoot);
}

/**
 * @brief Serialize level to file.
 * @param aParser
 */
void Level::Serialize(Parser &aParser)
{
  // Put menu objects into container.
  ObjectContainer menuObjects;
  for(MenuIT menuIT = mMenus.begin(); menuIT != mMenus.end(); ++menuIT)
  {
    MenuElement::ElementContainer elements = (*menuIT)->GetElements();
    for(MenuElement::ElementIT elementIT = elements.begin(); elementIT != elements.end(); ++elementIT)
    {
      menuObjects.insert((*elementIT)->GetObject());
    }
  }
  
  if(mGenerator)
    mGenerator->Serialize(aParser);
    
  // For each object not in a scenario, place in default scenario.
  for(ObjectIT it = mObjects[ObjectPlacement::DEFAULT].begin(); it != mObjects[ObjectPlacement::DEFAULT].end(); ++it)
  {
    if(ObjectNotInScenario(*it))
      mScenarios[mFileName].insert(*it);
  }
    
  SerializeObjects(aParser, mScenarios[mFileName], menuObjects);
  SerializeScenarios(aParser, menuObjects);
  
  // Music
  aParser.Place("Music", "");
  aParser.Place("Music", "Song", mMusicName);
  
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
 * @brief Serialize the tile map portion of level to another file.
 * @param aParser
 */
void Level::SerializeTileMap(Parser &aParser)
{
  if(!mGenerator)
  {
    DebugLogPrint("No TileMapGenerator available to Serialize in Level::SerializeTileMap");
    return;
  }
  aParser.Place("MapArtData", Common::IntVectorToString(mGenerator->GetArtTiles()));
  aParser.Place("Collision", Common::IntVectorToString(mGenerator->GetCollisionTiles()));
  aParser.Place("CollisionShapes", Common::IntVectorToString(mGenerator->GetCollisionShapes()));
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
          .set("LoadScenario", &Level::LoadScenario)
          .set("UnloadScenario", &Level::UnloadScenario)
          .set("UnloadScenarios", &Level::UnloadScenarios);
}

/**
 * @brief Parse file to create our level. Include game object component overrides.
 */
void Level::ParseFile(HashString const &aFileName, HashString const &aFolderName)
{
  TextParser parser(Common::RelativePath(aFolderName, aFileName).c_str());
  GameObject *object = nullptr;
  HashString const curObject = "Object_";
  int curIndex = 0;

  HashString tempIndex = curObject + Common::IntToString(curIndex);
  Root* curRoot = parser.Find(tempIndex);
  
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
    if(curRoot->Find("Text"))
    {
      ParseText(object, curRoot->Find("Text"));
    }
    // Get physics information
    if(curRoot->Find("PhysicsObject"))
    {
      ParsePhysicsObject(object, curRoot->Find("PhysicsObject"));
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
    // Who is the focus of this level?
    if(curRoot->Find("Focus"))
    {
      bool value = curRoot->Find("Focus")->Find("IsFocus")->GetValue().ToBool();
      if(value)
      {
        graphicsManager->GetScreen()->GetView().SetTarget(object);
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
    if(curRoot->Find("Parent"))
    {
      HashString parentName = curRoot->Find("Parent")->GetValue();
      GameObject *parent = FindObject(parentName);
      
      if(!parent)
        assert(!"Parent not found.");
      
      parent->AddChild(object);
      object->SetParent(parent);
    }

    RootContainer untouched = curRoot->GetUntouchedRoots();
    for(rootIT it = untouched.begin(); it != untouched.end(); ++it)
    {
      ParseAdditionalData(*it, object);
    }

    ++curIndex;
    tempIndex = curObject + Common::IntToString(curIndex);
    curRoot = parser.Find(tempIndex);
  }

  if(parser.Find("TileMapGenerator"))
  {
    ParseTileGenerator(parser);
  }
  if(parser.Find("Music"))
  {
    mMusicName = parser.Find("Music")->Find("Song")->GetValue().ToString();
  }
  if(parser.Find("Scenario"))
  {
    Root* scenario = parser.Find("Scenario");
    ParseFile(scenario->Find("Name")->GetValue(), scenario->Find("Folder")->GetValue());
    if(scenario->Find("Load")->GetValue().ToBool())
      LoadScenario(scenario->Find("Name")->GetValue());
  }
  if(parser.Find("Bounds"))
  {
    /*
     * Set camera bounds manually
     * 3 places where bounds can be set:
     * 1. Auto set from object transforms, see above
     * 2. Calculated from tilemap, see TileMapGenerator.cpp
     * 3. See below
     */
    int x, y;
    x = parser.Find("Bounds")->Find("MaxX")->GetValue().ToInt();
    y = parser.Find("Bounds")->Find("MaxY")->GetValue().ToInt();
    mMaxBoundary = Vector3(x, y, 0);
    x = parser.Find("Bounds")->Find("MinX")->GetValue().ToInt();
    y = parser.Find("Bounds")->Find("MinY")->GetValue().ToInt();
    mMinBoundary = Vector3(x, y, 0);
  }
  if(parser.Find("ClearColor"))
  {
    float r, g, b, a;
    r = parser.Find("ClearColor")->Find("ColorR")->GetValue().ToFloat();
    g = parser.Find("ClearColor")->Find("ColorG")->GetValue().ToFloat();
    b = parser.Find("ClearColor")->Find("ColorB")->GetValue().ToFloat();
    a = parser.Find("ClearColor")->Find("ColorA")->GetValue().ToFloat();
    mClearColor = Vector4(r, g, b, a);
  }
  if(parser.Find("Menu"))
  {
    // Menu adds itself to level.
    new Menu(this, parser.Find("Menu")->Find("Name")->GetValue());
  }

  RootContainer untouched = parser.GetBaseRoot()->GetUntouchedRoots();
  for(rootIT it = untouched.begin(); it != untouched.end(); ++it)
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
    DebugLogPrint("%s scenario file not found", aFileName.ToCharArray());
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
    DebugLogPrint("%s scenario file not found", aFileName.ToCharArray());
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
    DebugLogPrint("%s scenario file not found", aFileName.ToCharArray());
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
void Level::SerializeObjects(Parser &aParser, ObjectContainer &aObjects, ObjectContainer &aMenuObjects)
{
  int curIndex = 0;
  HashString object = "Object_";
  for(ObjectIT it = aObjects.begin(); it != aObjects.end(); ++it)
  {
    // Avoid menu objects
    if(std::find(aMenuObjects.begin(), aMenuObjects.end(), *it) != aMenuObjects.end())
      continue;
    
    HashString objectString = object + Common::IntToString(curIndex);
    aParser.SetCurrentObjectIndex(curIndex);
    aParser.Place(objectString, "");
    (*it)->Serialize(aParser);

    if(*it == mFocusTarget)
    {
      aParser.Place(objectString, "Focus", "");
      aParser.Find(objectString)->Place("Focus", "IsFocus", "true");
    }
    ++curIndex;
  }
}

/**
 * @brief Serialize all scenario files into separate chunks.
 * @param aParser Parser to get filename and directory from. (To be in same place as main level file.)
 */
void Level::SerializeScenarios(Parser &aParser, ObjectContainer &aMenuObjects)
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
    TextParser *parser = new TextParser(Common::RelativePath(folder, currentFileName + ".txt"), MODE_OUTPUT);
    SerializeObjects(*(Parser*)parser, it->second, aMenuObjects);
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
void Level::ParseTransform(GameObject *aObject, Root *aTransform)
{
  float posX, posY, posZ,
      scaleX, scaleY, scaleZ,
      sizeX, sizeY, sizeZ,
      rotateX, rotateY, rotateZ;
  posX = aTransform->Find("PositionX")->GetValue().ToFloat();
  posY = aTransform->Find("PositionY")->GetValue().ToFloat();
  posZ = aTransform->Find("PositionZ")->GetValue().ToFloat();
  scaleX = aTransform->Find("ScaleX")->GetValue().ToFloat();
  scaleY = aTransform->Find("ScaleY")->GetValue().ToFloat();
  scaleZ = aTransform->Find("ScaleZ")->GetValue().ToFloat();
  sizeX = aTransform->Find("SizeX")->GetValue().ToFloat();
  sizeY = aTransform->Find("SizeY")->GetValue().ToFloat();
  sizeZ = aTransform->Find("SizeZ")->GetValue().ToFloat();
  rotateX = aTransform->Find("RotationX")->GetValue().ToFloat();
  rotateY = aTransform->Find("RotationY")->GetValue().ToFloat();
  rotateZ = aTransform->Find("RotationZ")->GetValue().ToFloat();

  Transform* objTransform = aObject->GET<Transform>();
  objTransform->SetPosition(Vector3(posX,posY,posZ));
  objTransform->SetScale(Vector3(scaleX,scaleY,scaleZ));
  objTransform->SetSize(Vector3(sizeX,sizeY,sizeZ));
  
  Matrix33 rotation;
  rotation = rotation.Rotate(Vector3(1,0,0), rotateX);
  rotation = rotation.Rotate(Vector3(0,1,0), rotateY);
  rotation = rotation.Rotate(Vector3(0,0,1), rotateZ);
  objTransform->SetRotation(rotation);
  
  // Alignment (optional)
  if(aTransform->Find("AlignX"))
  {
    objTransform->SetXAlignment(X_ALIGN_CENTER);
    if(aTransform->Find("AlignX")->GetValue() == "LEFT")
      objTransform->SetXAlignment(X_ALIGN_LEFT);
    else if(aTransform->Find("AlignX")->GetValue() == "RIGHT")
      objTransform->SetXAlignment(X_ALIGN_RIGHT);
    else if(aTransform->Find("AlignX")->GetValue() != "CENTER")
      assert(!"Invalid value passed into XAlign");
  }
  if(aTransform->Find("AlignY"))
  {
    objTransform->SetYAlignment(Y_ALIGN_CENTER);
    if(aTransform->Find("AlignY")->GetValue() == "TOP")
      objTransform->SetYAlignment(Y_ALIGN_TOP);
    else if(aTransform->Find("AlignY")->GetValue() == "BOTTOM")
      objTransform->SetYAlignment(Y_ALIGN_BOTTOM);
    else if(aTransform->Find("AlignY")->GetValue() != "CENTER")
      assert(!"Invalid value passed into YAlign");
  }
  if(aTransform->Find("AlignZ"))
  {
    objTransform->SetZAlignment(Z_ALIGN_CENTER);
    if(aTransform->Find("AlignZ")->GetValue() == "FRONT")
      objTransform->SetZAlignment(Z_ALIGN_FRONT);
    else if(aTransform->Find("AlignZ")->GetValue() == "BACK")
      objTransform->SetZAlignment(Z_ALIGN_BACK);
    else if(aTransform->Find("AlignZ")->GetValue() != "CENTER")
      assert(!"Invalid value passed into ZAlign");
  }
  
  // Axis lock (optional)
  Root* axisLockRoot = aTransform->Find("LockedAxes");
  if(axisLockRoot)
  {
    AxisLock axisLock = NO_AXIS;
    HashString axisLockString = axisLockRoot->GetValue();
    DebugLogPrint("%s", axisLockString.ToCharArray());
    if(axisLockString == "X_AXIS")
      axisLock = X_AXIS;
    else if(axisLockString == "Y_AXIS")
      axisLock = Y_AXIS;
    else if(axisLockString == "Z_AXIS")
      axisLock = Z_AXIS;
    else if(axisLockString == "XY_AXIS")
      axisLock = XY_AXIS;
    else if(axisLockString == "YZ_AXIS")
      axisLock = YZ_AXIS;
    else if(axisLockString == "XZ_AXIS")
      axisLock = XZ_AXIS;
    else if(axisLockString == "ALL_AXES")
      axisLock = ALL_AXES;
    else if(axisLockString == "NO_AXIS")
      axisLock = NO_AXIS;
    else
      assert(!"Invalid axis lock value passed in.");
    objTransform->SetLockedAxis(axisLock);
  }
  
  // Parent inherit info
  Root* inheritNode = aTransform->Find("InheritInfo");
  if(inheritNode)
  {
    ParentInherit inheritance = INHERIT_ALL;
    HashString inheritInfo = inheritNode->GetValue();
    if(inheritInfo == "INHERIT_NONE")
      inheritance = INHERIT_NONE;
    else if(inheritInfo == "INHERIT_POSITION")
      inheritance = INHERIT_POSITION;
    else if(inheritInfo == "INHERIT_ROTATION")
      inheritance = INHERIT_ROTATION;
    else if(inheritInfo == "INHERIT_SCALE")
      inheritance = INHERIT_SCALE;
    else if(inheritInfo == "INHERIT_POSITION_ROTATION")
      inheritance = INHERIT_POSITION_ROTATION;
    else if(inheritInfo == "INHERIT_ROTATION_SCALE")
      inheritance = INHERIT_ROTATION_SCALE;
    else if(inheritInfo == "INHERIT_POSITION_SCALE")
      inheritance = INHERIT_POSITION_SCALE;
    else if(inheritInfo == "INHERIT_ALL")
      inheritance = INHERIT_ALL;
    else
      assert(!"Invalid inheritance value passed in.");
    objTransform->SetParentInheritanceInfo(inheritance);
  }

  // Auto set camera bounds based on objects in environment
  mMinBoundary.x = Lesser<float>(posX - sizeX, mMinBoundary.x);
  mMinBoundary.y = Lesser<float>(posY - sizeY, mMinBoundary.y);
  mMaxBoundary.x = Greater<float>(posX + sizeX, mMaxBoundary.x);
  mMaxBoundary.y = Greater<float>(posY + sizeY, mMaxBoundary.y);
}

/**
 * @brief Get surface data from a root.
 */
void Level::ParseSurface(GameObject *aObject, Root *aSurface)
{
  Surface* objSurface = aObject->GET<Surface>();
  if(aSurface->Find("ColorR"))
  {
    float r, g, b, a;
    r = aSurface->Find("ColorR")->GetValue().ToFloat();
    g = aSurface->Find("ColorG")->GetValue().ToFloat();
    b = aSurface->Find("ColorB")->GetValue().ToFloat();
    a = aSurface->Find("ColorA")->GetValue().ToFloat();
    objSurface->SetColor(Vector4(r, g, b, a));
  }
  if(aSurface->Find("StartingAnimation"))
  {
    int startingAnimation = aSurface->Find("StartingAnimation")->GetValue().ToInt();
    objSurface->SetAnimation(startingAnimation);
  }
  if(aSurface->Find("ViewMode"))
  {
    HashString viewMode = aSurface->Find("ViewMode")->GetValue();
    if(viewMode == "ABSOLUTE")
      objSurface->SetViewMode(VIEW_ABSOLUTE);
    else if(viewMode == "RELATIVE")
      objSurface->SetViewMode(VIEW_RELATIVE_TO_CAMERA);
    else if(viewMode == "PERCENTAGE")
      objSurface->SetViewMode(VIEW_PERCENTAGE_OF_CAMERA);
    else
      assert(!"Invalid value passed into ViewMode for Surface. (Level.cpp)");  
  }
#ifdef SHADER_COMPATIBLE
  if(aSurface->Find("VertexShader") && aSurface->Find("FragmentShader"))
  {
    objSurface->LoadShaders(aSurface->Find("VertexShader")->GetValue(), aSurface->Find("FragmentShader")->GetValue());
  }
#endif
}

/**
 * @brief Get text data from root.
 */
void Level::ParseText(GameObject *aObject, Root* aText)
{
  // TODO
}

/**
 * @brief Get physics object data from a root.
 */
void Level::ParsePhysicsObject(GameObject *aObject, Root* aPhysicsObject)
{
  // If object doesn't have physicsobject, it does now.
  PhysicsObject* physicsObject = aObject->GET<PhysicsObject>();
  if(!physicsObject)
  {
    physicsObject = new PhysicsObject(GetManager()->GetOwningApp()->GET<PhysicsWorld>());
    aObject->AddComponent(physicsObject);
  }
  
  // Clear out current shapes.
  std::vector<Shape*>& shapes = physicsObject->GetShapes();
  for(std::vector<Shape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
  {
    delete *it;
  }
  shapes.clear();
  
  // Serialize the physicsobject
  bool gravity = aPhysicsObject->Find("Gravity")->GetValue().ToBool();
  bool isStatic = aPhysicsObject->Find("Static")->GetValue().ToBool();
  bool isPassable = aPhysicsObject->Find("Passable")->GetValue().ToBool();
  physicsObject->SetMass(aPhysicsObject->Find("Mass")->GetValue().ToInt());
  physicsObject->SetDamping(aPhysicsObject->Find("Damping")->GetValue().ToFloat());
  
  // Optional
  if(aPhysicsObject->Find("IgnoreList"))
  {
    std::vector<std::string> ignoreList = aPhysicsObject->Find("IgnoreList")->GetValue().ToStringVector();
    PhysicsObject::IgnoreContainer ignoreContainer;
    
    for(std::vector<std::string>::iterator it = ignoreList.begin(); it != ignoreList.end(); ++it)
    {
      ignoreContainer[Common::StringHashFunction(*it)] = *it;
    }
    physicsObject->SetIgnoreList(ignoreContainer);
  }
  if(aPhysicsObject->Find("MaxVelocity"))
  {
    physicsObject->SetMaxVelocity(aPhysicsObject->Find("MaxVelocity")->GetValue().ToFloat());
  }
  
  // Default true
  if(!gravity)
  {
    GetManager()->GetOwningApp()->GET<PhysicsWorld>()->UnregisterGravity(physicsObject);
    physicsObject->SetAffectedByGravity(false);
  }
  
  physicsObject->SetStatic(isStatic);
  physicsObject->SetPassable(isPassable);
  
  HashString const SHAPE = "Shape_";
  int curIndex = 0;
  HashString curShape = SHAPE + Common::IntToString(curIndex);
  
  // Adding shapes
  // NOTE: ALL SHAPE POSITIONS ARE IN LOCAL SPACE
  while(aPhysicsObject->Find(curShape))
  {
    Root* tempShape = aPhysicsObject->Find(curShape);
    Shape* newShape = nullptr;
    
    HashString type = tempShape->Find("Type")->GetValue();
    
    // Discern type and serialize accordingly
    if(type == "CUBE")
    {
      newShape = new Cube();
      Cube* cube = (Cube*)newShape;
      cube->size = Vector3(tempShape->Find("SizeX")->GetValue().ToFloat(),
                          tempShape->Find("SizeY")->GetValue().ToFloat(),
                          tempShape->Find("SizeZ")->GetValue().ToFloat());
      cube->shape = Shape::CUBE;
    }
    else if(type == "SPHERE")
    {
      newShape = new Sphere();
      Sphere* sphere = (Sphere*)newShape;
      sphere->radius = tempShape->Find("Radius")->GetValue().ToFloat();
      sphere->shape = Shape::SPHERE;
    }
    else if(type == "TRIANGLE")
    {
      newShape = new Triangle();
      Triangle* triangle = (Triangle*)newShape;
      HashString point = "Point_";
      for(int i = 0; i < 3; ++i)
      {
        HashString pointId = point + Common::IntToString(i);
        Vector3 point(tempShape->Find(pointId + "X")->GetValue().ToFloat(),
                      tempShape->Find(pointId + "Y")->GetValue().ToFloat(),
                      tempShape->Find(pointId + "Z")->GetValue().ToFloat());
        triangle->points[i] = point;
      }
      triangle->shape = Shape::TRIANGLE;
    }
    else if(type == "LINE")
    {
      newShape = new Line();
      Line *line = (Line*)newShape;
      line->direction = Vector3(tempShape->Find("DirectionX")->GetValue().ToFloat(),
                                tempShape->Find("DirectionY")->GetValue().ToFloat(),
                                tempShape->Find("DirectionZ")->GetValue().ToFloat());
      line->length = tempShape->Find("Length")->GetValue().ToFloat();
      line->shape = Shape::LINE;
    }
    else
      assert(!"Invalid shape given");
      
    newShape->id = curIndex;
    newShape->position = Vector3(tempShape->Find("PositionX")->GetValue().ToFloat(),
                                 tempShape->Find("PositionY")->GetValue().ToFloat(),
                                 tempShape->Find("PositionZ")->GetValue().ToFloat());
                                 
    // If passable flag is found, use it. Default false.
    if(tempShape->Find("Passable"))
    {
      newShape->passable = tempShape->Find("Passable")->GetValue().ToBool();
    }
    physicsObject->AddShape(newShape);
    
    ++curIndex;
    curShape = SHAPE + Common::IntToString(curIndex);
  }
}

/**
 * @brief Get chemistry material data from root.
 */
void Level::ParseChemistryMaterial(GameObject *aObject, Root* aChemistryMaterial)
{
  // If object doesn't have chemistryMaterial, it does now.
  ChemistryMaterial* chemistryMaterial = aObject->GET<ChemistryMaterial>();
  if(!chemistryMaterial)
  {
    chemistryMaterial = GetManager()->GetOwningApp()->GET<ChemistryManager>()->CreateMaterial(aChemistryMaterial->Find("Name")->GetValue());
    aObject->AddComponent(chemistryMaterial);
  }
  
  // Serialize the chemistryMaterial 
  if(aChemistryMaterial->Find("Name"))
  {
    HashString name = aChemistryMaterial->Find("Name")->GetValue();
    chemistryMaterial->SetName(name);
  }
  if(aChemistryMaterial->Find("BoilingPoint"))
  {
    float boilingPoint = aChemistryMaterial->Find("BoilingPoint")->GetValue().ToFloat();
    chemistryMaterial->SetBoilingPoint(boilingPoint);
  }
  if(aChemistryMaterial->Find("MeltingPoint"))
  {
    float meltingPoint = aChemistryMaterial->Find("MeltingPoint")->GetValue().ToFloat();
    chemistryMaterial->SetMeltingPoint(meltingPoint);
  }
  if(aChemistryMaterial->Find("FreezingPoint"))
  {
    float freezingPoint = aChemistryMaterial->Find("FreezingPoint")->GetValue().ToFloat();
    chemistryMaterial->SetFreezingPoint(freezingPoint);
  }
  if(aChemistryMaterial->Find("Conductivity"))
  {
    float conductivity = aChemistryMaterial->Find("Conductivity")->GetValue().ToFloat();
    chemistryMaterial->SetConductivity(conductivity);
  }
  if(aChemistryMaterial->Find("HeatTransferRate"))
  {
    float heatTransferRate = aChemistryMaterial->Find("HeatTransferRate")->GetValue().ToFloat();
    chemistryMaterial->SetHeatTransferRate(heatTransferRate);
  }
  if(aChemistryMaterial->Find("StartingTemperature"))
  {
    float temperature = aChemistryMaterial->Find("StartingTemperature")->GetValue().ToFloat();
    chemistryMaterial->SetCurrentTemperature(temperature);
  }
  if(aChemistryMaterial->Find("StartingWattage"))
  {
    float wattage = aChemistryMaterial->Find("StartingWattage")->GetValue().ToFloat();
    chemistryMaterial->SetCurrentWattage(wattage);
  }
}

/**
 * @brief Get chemistry element data from root.
 */
void Level::ParseChemistryElement(GameObject *aObject, Root* aChemistryElement)
{
  // If object doesn't have chemistryElement, it does now.
  ChemistryElement* chemistryElement = aObject->GET<ChemistryElement>();
  if(!chemistryElement)
  {
    chemistryElement = GetManager()->GetOwningApp()->GET<ChemistryManager>()->CreateElement(aChemistryElement->Find("Name")->GetValue());
    aObject->AddComponent(chemistryElement);
  }
  
  // Serialize the chemistryElement
  if(aChemistryElement->Find("Name"))
  {
    HashString name = aChemistryElement->Find("Name")->GetValue();
    chemistryElement->SetName(name);
  }
  if(aChemistryElement->Find("Temperature"))
  {
    float temperature = aChemistryElement->Find("Temperature")->GetValue().ToFloat();
    chemistryElement->SetTemperature(temperature);
  }
  if(aChemistryElement->Find("Wattage"))
  {
    float wattage = aChemistryElement->Find("Wattage")->GetValue().ToFloat();
    chemistryElement->SetWattage(wattage);
  }
  if(aChemistryElement->Find("Scale"))
  {
    float scale = aChemistryElement->Find("Scale")->GetValue().ToFloat();
    chemistryElement->SetScale(scale);
  }
  if(aChemistryElement->Find("Falloff"))
  {
    float falloff = aChemistryElement->Find("Falloff")->GetValue().ToFloat();
    chemistryElement->SetFalloff(falloff);
  }
  if(aChemistryElement->Find("DirectionX"))
  {
    float directionX = aChemistryElement->Find("DirectionX")->GetValue().ToFloat();
    float directionY = aChemistryElement->Find("DirectionY")->GetValue().ToFloat();
    float directionZ = aChemistryElement->Find("DirectionZ")->GetValue().ToFloat();
    Vector3 direction = Vector3(directionX, directionY, directionZ);
    chemistryElement->SetDirectionality(direction);
  }
}

/**
 * @brief Create tile generator helper.
 * @param aParser
 */
void Level::ParseTileGenerator(TextParser &aParser)
{
  Root* tileMap = aParser.Find("TileMapGenerator");
  HashString value, empty;
  int width, height, tileSize;
  HashString file, frameDataFilename, settingsDataFileName;
  std::vector<int> frames, collision, shapes, materials;
  std::map<int, float> heights;
  std::map<int, std::vector<int>> animations;
  std::map<int, HashString> materialInfo;
  float tileAnimationSpeed = 0.0f;

  width = tileMap->Find("Width")->GetValue().ToInt();
  height = tileMap->Find("Height")->GetValue().ToInt();
  tileSize = tileMap->Find("TileSize")->GetValue().ToInt();
  file = tileMap->Find("Image")->GetValue();
  frameDataFilename = tileMap->Find("Data")->GetValue();
  
  // Reserve the tilemap data to avoid reallocs.
  int totalAllocation = width * height;
  frames.reserve(totalAllocation);
  collision.reserve(totalAllocation);
  shapes.reserve(totalAllocation);
  
  // If there's setting data, parse it in and use it.
  if(tileMap->Find("SettingsFile"))
  {
    HashString const height = "Height_";
    HashString const animation = "Animation_";
    HashString const material = "Material_";
    settingsDataFileName = tileMap->Find("SettingsFile")->GetValue();
    TextParser settingsData(Common::RelativePath("Maps", settingsDataFileName));
    
    // Heightmap
    int index = 0;
    HashString curIndex = height + Common::IntToString(index);
    while(settingsData.Find(curIndex))
    {
      std::vector<float> values = Common::StringToFloatVector(settingsData.Find(curIndex)->GetValue());
      heights[static_cast<int>(values[0])] = values[1];
      ++index;
      curIndex = height + Common::IntToString(index);
    }

    // Animations
    index = 0;
    curIndex = animation + Common::IntToString(index);
    while(settingsData.Find(curIndex))
    {
      int tileID = settingsData.Find(curIndex)->Find("TileID")->GetValue().ToInt();
      std::vector<int> animationData = settingsData.Find(curIndex)->Find("Animation")->GetValue().ToIntVector();
      animations[tileID] = animationData;
      ++index;
      curIndex = animation + Common::IntToString(index);
    }
    
    // Materials
    index = 0;
    curIndex = material + Common::IntToString(index);
    while(settingsData.Find(curIndex))
    {
      Root* base = settingsData.Find(curIndex);
      HashString name = base->Find("Name")->GetValue();
      materialInfo[index] = name;
      ++index;
      curIndex = material + Common::IntToString(index);
    }

    // Animation speed
    if(settingsData.Find("AnimationSpeed"))
    {
      tileAnimationSpeed = settingsData.Find("AnimationSpeed")->GetValue().ToFloat();
    }
  }

  TextParser tileMapData(Common::RelativePath("Maps", frameDataFilename));
  frames = Common::StringToIntVector(tileMapData.Find("MapArtData")->GetValue());
  collision = Common::StringToIntVector(tileMapData.Find("Collision")->GetValue());
  
  if(tileMapData.Find("CollisionShapes"))
  {
    shapes = Common::StringToIntVector(tileMapData.Find("CollisionShapes")->GetValue());
  }
  if(tileMapData.Find("Materials"))
  {
    materials = Common::StringToIntVector(tileMapData.Find("Materials")->GetValue());
  }

  mGenerator = new TileMapGenerator(width, height, tileSize,
                                   file, frameDataFilename,
                                   frames, collision, shapes,
                                   materials, heights, materialInfo,
                                   animations, tileAnimationSpeed, this);
}
