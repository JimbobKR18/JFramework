#include "Level.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "MathExt.h"
#include "Transform.h"
#include "GraphicsManager.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "ControllerManager.h"
#include "SoundManager.h"
#include "TileMapGenerator.h"
#include "Menu.h"
#include "Common.h"
#include "LuaIncludes.h"
#include "ObjectDeleteMessage.h"
#include "ObjectCreateMessage.h"
#include "ResetLevelMessage.h"

bool SortPredicate(GameObject *aLhs, GameObject *aRhs)
{
  return aLhs->GET<Transform>()->GetPosition().z <= aRhs->GET<Transform>()->GetPosition().z;
}

Level::Level()
{
	assert(0);
}

Level::Level(LevelManager *aManager, std::string const &aFileName, bool aAutoParse) :
             mName(""), mFileName(aFileName), mMusicName(""), mObjects(),
             mStaticObjects(), mMenus(), mOwner(aManager), mGenerator(NULL),
             mFocusTarget(NULL), mActive(false), mMaxBoundary(0,0,0), mMinBoundary(0,0,0)
{
  for(int i = static_cast<int>(aFileName.size()) - 1;
      i >= 0 && aFileName[i] != '/'; --i)
  {
    mName.push_back(aFileName[i]);
  }
  std::reverse(mName.begin(), mName.end());
  mName = mName.substr(0, mName.size() - 4);

  if(aAutoParse)
    ParseFile();
}

Level::~Level()
{
  if(mActive)
  {
    mObjects.clear();
    mStaticObjects.clear();
  }
  else
    DeleteObjects();

  if(mGenerator)
    delete mGenerator;
}

/**
 * @brief Get the level name. Not the file name.
 * @return The level name.
 */
std::string Level::GetName() const
{
  return mName;
}

/**
* @brief Get the file name, not the level name.
* @return The file name.
*/
std::string Level::GetFileName() const
{
  return mFileName;
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
GameObject* Level::FindObject(std::string const &aObjectName)
{
  // This is designed to be naive
  // if two objects share the same name...
  // well, too bad.
  ObjectIT end = mObjects.end();
  for(ObjectIT it = mObjects.begin(); it != end; ++it)
  {
    if(aObjectName == (*it)->GetName())
      return *it;
  }
  end = mStaticObjects.end();
  for(ObjectIT it = mStaticObjects.begin(); it != end; ++it)
  {
    if(aObjectName == (*it)->GetName())
      return *it;
  }
  return nullptr;
}

/**
 * @brief Find all objects touching a position
 * @param aPosition
 * @return A vector of objects touching the location, can be empty.
 */
Level::ObjectContainer Level::FindObjects(Vector3 const &aPosition) const
{
  ObjectContainer ret;
  for(ConstObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
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
Menu* Level::FindMenu(std::string const &aMenuName)
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
  if(aMenu == NULL)
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
  
  #ifdef _DEBUG
    assert(!"Menu not found, you sure you added it to this list?");
  #endif
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
void Level::AddObject(GameObject *aObject)
{
  ObjectIT objectsEnd = mObjects.end();
  for(ObjectIT it = mObjects.begin(); it != objectsEnd; ++it)
	{
		if(*it == aObject)
		{
      return;
		}
	}
  mObjects.push_back(aObject);
}

/**
 * @brief Add an object that's not meant to be updated.
 * @param aObject
 */
void Level::AddStaticObject(GameObject *aObject)
{
  ObjectIT staticObjectsEnd = mStaticObjects.end();
  for(ObjectIT it = mStaticObjects.begin(); it != staticObjectsEnd; ++it)
  {
    if(*it == aObject)
    {
      return;
    }
  }
  mStaticObjects.push_back(aObject);
}

/**
 * @brief Delete object and remove from our objects vector.
 * @param aObject
 */
void Level::DeleteObject(GameObject *aObject)
{
  ObjectManager *objectManager = mOwner->GetOwningApp()->GET<ObjectManager>();
  GraphicsManager *graphicsManager = mOwner->GetOwningApp()->GET<GraphicsManager>();
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    if(aObject == *it)
    {
      mObjects.erase(it);
      objectManager->DeleteObject(aObject);
      break;
    }
  }
  for(ObjectIT it = mStaticObjects.begin(); it != mStaticObjects.end(); ++it)
  {
    if(aObject == *it)
    {
      mStaticObjects.erase(it);
      objectManager->DeleteObject(aObject);
      break;
    }
  }
  
  // Unassociate object from view target if need be
  if(graphicsManager->GetScreen()->GetView().GetTarget() == aObject)
  {
    graphicsManager->GetScreen()->GetView().SetTarget(nullptr);
  }
}

/**
 * @brief Create an object to be added next frame.
 * @param aFileName
 * @return The newly created object.
 */
GameObject* Level::CreateObjectDelayed(HashString const &aFileName)
{
  ObjectManager *objectManager = mOwner->GetOwningApp()->GET<ObjectManager>();
  GameObject *object = objectManager->CreateObjectNoAdd(aFileName);
  ObjectCreateMessage *msg = new ObjectCreateMessage(object);
  objectManager->ProcessDelayedMessage(msg);
  AddObject(object);
  return object;
}

/**
 * @brief Marks an object for deletion next frame.
 * @param aObject
 */
void Level::DeleteObjectDelayed(GameObject *aObject)
{
  ObjectManager *objectManager = mOwner->GetOwningApp()->GET<ObjectManager>();
  GraphicsManager *graphicsManager = mOwner->GetOwningApp()->GET<GraphicsManager>();
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    if(aObject == *it)
    {
      mObjects.erase(it);
      ObjectDeleteMessage *msg = new ObjectDeleteMessage(aObject);
      objectManager->ProcessDelayedMessage(msg);
      break;
    }
  }
  for(ObjectIT it = mStaticObjects.begin(); it != mStaticObjects.end(); ++it)
  {
    if(aObject == *it)
    {
      mStaticObjects.erase(it);
      ObjectDeleteMessage *msg = new ObjectDeleteMessage(aObject);
      objectManager->ProcessDelayedMessage(msg);
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
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    manager->DeleteObject(*it);
  }
  for(ObjectIT it = mStaticObjects.begin(); it != mStaticObjects.end(); ++it)
  {
    manager->DeleteObject(*it);
  }
  for(MenuIT it = mMenus.begin(); it != mMenus.end(); ++it)
  {
    delete *it;
  }
  mMenus.clear();
  mObjects.clear();
  mStaticObjects.clear();
  manager->ClearObjects();
  manager->GetOwningApp()->ClearDelayedMessages();
}

/**
 * @brief Delete objects and start level over.
 */
void Level::Reset()
{
  GetManager()->ProcessDelayedMessage(new ResetLevelMessage());
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
  ParseFile();
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
void Level::Load(Level* const aPrevLevel)
{
  // Load all objects
  LoadObjects(mObjects, false);
  LoadObjects(mStaticObjects, true);

  if(!mMusicName.empty() && (!aPrevLevel || aPrevLevel->mMusicName != mMusicName))
    mOwner->GetOwningApp()->GET<SoundManager>()->PlaySound(mMusicName, Sound::INFINITE_LOOPS);

  mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(mFocusTarget);
  mActive = true;

  mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetMaxBoundary(mMaxBoundary);
  mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetMinBoundary(mMinBoundary);
  
  mOwner->SetActiveLevel(this);
}

/**
 * @brief Specify all objects to be be displayed / updated.
 * @param aNextLevel
 */
void Level::Unload(Level* const aNextLevel)
{
  // Remove menus because they are not level files.
  RemoveMenus();
  
  // Unload all objects
  UnloadObjects(mObjects);
  UnloadObjects(mStaticObjects);

  if(!mMusicName.empty() && (!aNextLevel || aNextLevel->mMusicName != mMusicName))
    mOwner->GetOwningApp()->GET<SoundManager>()->StopSound(mMusicName);

  mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(NULL);
  mActive = false;
  
  mOwner->SetActiveLevel(NULL);
}

/**
 * @brief Load all objects in list into view.
 * @param aObjects Objects to add.
 */
void Level::LoadObjects(ObjectContainer const &aObjects, bool const aStatic)
{
  for(ConstObjectIT it = aObjects.begin(); it != aObjects.end(); ++it)
  {
    mOwner->GetOwningApp()->GET<ObjectManager>()->AddObject(*it, aStatic);
    if((*it)->GET<PhysicsObject>())
      mOwner->GetOwningApp()->GET<PhysicsWorld>()->AddObject((*it)->GET<PhysicsObject>());
    if((*it)->GET<Surface>())
      mOwner->GetOwningApp()->GET<GraphicsManager>()->AddSurface((*it)->GET<Surface>());
    if((*it)->GET<Controller>())
      mOwner->GetOwningApp()->GET<ControllerManager>()->AddController((*it)->GET<Controller>());
  }
}

/**
 * @brief Unload all objects in the list from view, they will still exist in then level file.
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
  }
}

/**
 * @brief Update loop for levels. LevelManager will call this for you.
 */
void Level::Update()
{
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
  DebugLogPrint("Root %s skipped, are you trying to read in additional data for a new level type?", aRoot->GetName().ToCharArray());
}

/**
 * @brief Serialize level to file.
 * @param aParser
 */
void Level::Serialize(Parser &aParser)
{
  int curIndex = 0;
  std::string object = "Object_";
  
  if(mGenerator)
    mGenerator->Serialize(aParser);
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it, ++curIndex)
  {
    std::string objectString = object + Common::IntToString(curIndex);
    aParser.SetCurrentObjectIndex(curIndex);
    aParser.Place(objectString, "");
    (*it)->Serialize(aParser);

    if(*it == mFocusTarget)
    {
      aParser.Place(objectString, "Focus", "");
      aParser.Find(objectString)->Place("Focus", "IsFocus", "true");
    }
  }
  /*for(ObjectIT it = mStaticObjects.begin(); it != mStaticObjects.end(); ++it)
  {
    (*it)->Serialize(aParser);
  }*/
  // TODO focus target
  aParser.Place("Music", "");
  aParser.Place("Music", "Song", mMusicName);
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
          .set("DeleteObject", &Level::DeleteObjectDelayed);
}

/**
 * @brief Parse file to create our level. Include game object component overrides.
 */
void Level::ParseFile()
{
  TextParser parser(Common::RelativePath("Game", mFileName).c_str());
  GameObject *object = NULL;
  HashString const curObject = "Object_";
  int curIndex = 0;

  HashString tempIndex = curObject + Common::IntToString(curIndex);
  Root* curRoot = parser.Find(tempIndex);

  // While there are objects to find.
  while(curRoot)
  {
    // Make Object to assign params to
    ObjectManager *manager = mOwner->GetOwningApp()->GET<ObjectManager>();
    object = new GameObject(manager, curRoot->Find("File")->GetValue());
    manager->ParseObject(object);
    mObjects.push_back(object);

    // TODO PhysicsObject serialization
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
    if(curRoot->Find("PhysicsObject"))
    {
      ParsePhysicsObject(object, curRoot->Find("PhysicsObject"));
    }
    // Who is the focus of this level?
    if(curRoot->Find("Focus"))
    {
      bool value = curRoot->Find("Focus")->Find("IsFocus")->GetValue().ToBool();
      if(value)
      {
        mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(object);
        mFocusTarget = object;
      }
    }
    if(curRoot->Find("Name"))
    {
      object->SetName(curRoot->Find("Name")->GetValue());
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

  RootContainer untouched = parser.GetBaseRoot()->GetUntouchedRoots();
  for(rootIT it = untouched.begin(); it != untouched.end(); ++it)
  {
    ParseAdditionalData(*it, nullptr);
  }
}

/**
 * @brief Get all objects in level.
 * @return All objects.
 */
Level::ObjectContainer& Level::GetObjects()
{
  return mObjects;
}

/**
 * @brief Get all static objects in level.
 * @return All static objects.
 */
Level::ObjectContainer& Level::GetStaticObjects()
{
  return mStaticObjects;
}

/**
 * @brief Get transform data for an object from a root.
 * @param aTransform
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

  // Auto set camera bounds based on objects in environment
  mMinBoundary.x = Lesser<float>(posX - sizeX, mMinBoundary.x);
  mMinBoundary.y = Lesser<float>(posY - sizeY, mMinBoundary.x);
  mMaxBoundary.x = Greater<float>(posX + sizeX, mMaxBoundary.x);
  mMaxBoundary.y = Greater<float>(posY + sizeY, mMaxBoundary.x);
}

/**
 * @brief Get surface data from a root.
 * @param aSurface
 */
void Level::ParseSurface(GameObject *aObject, Root *aSurface)
{
  float r, g, b, a;
  r = aSurface->Find("ColorR")->GetValue().ToFloat();
  g = aSurface->Find("ColorG")->GetValue().ToFloat();
  b = aSurface->Find("ColorB")->GetValue().ToFloat();
  a = aSurface->Find("ColorA")->GetValue().ToFloat();

  Surface* objSurface = aObject->GET<Surface>();
  objSurface->SetColor(Vector4(r, g, b, a));
}

/**
 * @brief Get physics object data from a root.
 * @param aPhysicsObject
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
  
  // default true
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
    }
    else if(type == "SPHERE")
    {
      newShape = new Sphere();
      Sphere* sphere = (Sphere*)newShape;
      sphere->radius = tempShape->Find("Radius")->GetValue().ToFloat();
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
    }
    else
      assert(!"Invalid shape given");
      
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
 * @brief Create tile generator helper.
 * @param aParser
 */
void Level::ParseTileGenerator(TextParser &aParser)
{
  Root* tileMap = aParser.Find("TileMapGenerator");
  HashString value, empty;
  int width, height, tileSize;
  HashString file, frameDataFilename, settingsDataFileName;
  std::vector<int> frames, collision, shapes;
  std::map<int, float> heights;
  std::map<int, std::vector<int>> animations;
  float tileAnimationSpeed = 0.0f;

  width = tileMap->Find("Width")->GetValue().ToInt();
  height = tileMap->Find("Height")->GetValue().ToInt();
  tileSize = tileMap->Find("TileSize")->GetValue().ToInt();
  file = tileMap->Find("Image")->GetValue();
  frameDataFilename = tileMap->Find("Data")->GetValue();
  
  // Reserve the tilemap data to avoid reallocs.
  int totalAllocation = width * height;
  mStaticObjects.reserve(totalAllocation);
  frames.reserve(totalAllocation);
  collision.reserve(totalAllocation);
  shapes.reserve(totalAllocation);
  
  // If there's setting data, parse it in and use it.
  if(tileMap->Find("SettingsFile"))
  {
    HashString const height = "Height_";
    HashString const animation = "Animation_";
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

  mGenerator = new TileMapGenerator(width, height, tileSize,
                                   file, frameDataFilename,
                                   frames, collision, shapes,
                                   heights, animations, 
                                   tileAnimationSpeed, this);
}