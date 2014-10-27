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

bool SortPredicate(GameObject *aLhs, GameObject *aRhs)
{
  return aLhs->GET<Transform>()->GetPosition().z <= aRhs->GET<Transform>()->GetPosition().z;
}

Level::Level()
{
	assert(0);
}

Level::Level(LevelManager *aManager, std::string const &aFileName, bool aAutoParse) :
             mName(""), mMusicName(""), mObjects(),
             mStaticObjects(), mMenus(), mOwner(aManager), mGenerator(NULL),
             mFocusTarget(NULL), mActive(false), mMaxBoundary(0,0,0), mMinBoundary(0,0,0)
{
  for(int i = static_cast<int>(aFileName.size()) - 1;
      aFileName[i] != '/' && i >= 0; --i)
  {
    mName.push_back(aFileName[i]);
  }
  std::reverse(mName.begin(), mName.end());

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

std::string Level::GetName() const
{
	return mName;
}

LevelManager *Level::GetManager() const
{
  return mOwner;
}

TileMapGenerator* Level::GetTileMap() const
{
  return mGenerator;
}

GameObject* Level::GetFocusTarget() const
{
  return mFocusTarget;
}

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
  return NULL;
}

Level::ObjectContainer Level::FindObjects(Vector3 const &aPosition) const
{
  ObjectContainer ret;
  for(ConstObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    Transform* transform = (*it)->GET<Transform>();
    Cube cube(transform->GetPosition(), transform->GetSize());

    if(cube.GetCollision(aPosition))
    {
      ret.push_back(*it);
    }
  }
  std::sort(ret.begin(), ret.end(), SortPredicate);
  return ret;
}

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

void Level::AddMenu(Menu *aMenu)
{
  if(!FindMenu(aMenu->GetName()))
    mMenus.push_back(aMenu);
}

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
  assert(!"Menu not found, you sure you added it to this list?");
}

void Level::RemoveMenus()
{
  for(MenuIT it = mMenus.begin(); it != mMenus.end(); ++it)
  {
    delete *it;
  }
  mMenus.clear();
}

void Level::AddObject(GameObject *aObject)
{
  mObjects.push_back(aObject);
}

void Level::AddStaticObject(GameObject *aObject)
{
  mStaticObjects.push_back(aObject);
}

void Level::DeleteObject(GameObject *aObject)
{
  ObjectManager *manager = mOwner->GetOwningApp()->GET<ObjectManager>();
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    if(aObject == *it)
    {
      mObjects.erase(it);
      manager->DeleteObject(aObject);
      break;
    }
  }
  for(ObjectIT it = mStaticObjects.begin(); it != mStaticObjects.end(); ++it)
  {
    if(aObject == *it)
    {
      mStaticObjects.erase(it);
      manager->DeleteObject(aObject);
      break;
    }
  }
}

void Level::DeleteObjectDelayed(GameObject *aObject)
{
  ObjectManager *manager = mOwner->GetOwningApp()->GET<ObjectManager>();
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    if(aObject == *it)
    {
      mObjects.erase(it);
      ObjectDeleteMessage *msg = new ObjectDeleteMessage(aObject);
      manager->ProcessDelayedMessage(msg);
      break;
    }
  }
  for(ObjectIT it = mStaticObjects.begin(); it != mStaticObjects.end(); ++it)
  {
    if(aObject == *it)
    {
      mStaticObjects.erase(it);
      ObjectDeleteMessage *msg = new ObjectDeleteMessage(aObject);
      manager->ProcessDelayedMessage(msg);
      break;
    }
  }
}

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
  mObjects.clear();
  mStaticObjects.clear();
}

void Level::Reset()
{
  PreReset();
  DeleteObjects();
  ParseFile();
}

void Level::SetMaxBoundary(Vector3 const &aMaxBoundary)
{
  mMaxBoundary = aMaxBoundary;
}

void Level::SetMinBoundary(Vector3 const &aMinBoundary)
{
  mMinBoundary = aMinBoundary;
}

Vector3 Level::GetMaxBoundary() const
{
  return mMaxBoundary;
}

Vector3 Level::GetMinBoundary() const
{
  return mMinBoundary;
}

void Level::Load(Level* const aPrevLevel)
{
	for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		mOwner->GetOwningApp()->GET<ObjectManager>()->AddObject(*it);
    if((*it)->GET<PhysicsObject>())
      mOwner->GetOwningApp()->GET<PhysicsWorld>()->AddObject((*it)->GET<PhysicsObject>());
    if((*it)->GET<Surface>())
      mOwner->GetOwningApp()->GET<GraphicsManager>()->AddSurface((*it)->GET<Surface>());
    if((*it)->GET<Controller>())
      mOwner->GetOwningApp()->GET<ControllerManager>()->AddController((*it)->GET<Controller>());
	}
	for(ObjectIT it = mStaticObjects.begin(); it != mStaticObjects.end(); ++it)
  {
    mOwner->GetOwningApp()->GET<ObjectManager>()->AddObject(*it, true);
    if((*it)->GET<PhysicsObject>())
      mOwner->GetOwningApp()->GET<PhysicsWorld>()->AddObject((*it)->GET<PhysicsObject>());
    if((*it)->GET<Surface>())
      mOwner->GetOwningApp()->GET<GraphicsManager>()->AddSurface((*it)->GET<Surface>());
    if((*it)->GET<Controller>())
      mOwner->GetOwningApp()->GET<ControllerManager>()->AddController((*it)->GET<Controller>());
  }

	if(!mMusicName.empty() && (!aPrevLevel || aPrevLevel->mMusicName != mMusicName))
	  mOwner->GetOwningApp()->GET<SoundManager>()->PlaySound(mMusicName);

	mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(mFocusTarget);
	mActive = true;

	mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetMaxBoundary(mMaxBoundary);
	mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetMinBoundary(mMinBoundary);
  
  mOwner->SetActiveLevel(this);
}

void Level::Unload(Level* const aNextLevel)
{
  // Remove menus because they are not level files.
  RemoveMenus();
  
	for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
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
	for(ObjectIT it = mStaticObjects.begin(); it != mStaticObjects.end(); ++it)
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

	if(!mMusicName.empty() && (!aNextLevel || aNextLevel->mMusicName != mMusicName))
	  mOwner->GetOwningApp()->GET<SoundManager>()->StopSound(mMusicName);

	mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(NULL);
	mActive = false;
  
  mOwner->SetActiveLevel(NULL);
}

void Level::Serialize(Parser &aParser)
{
  int curIndex = 0;
  std::string object = "Object_";
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

void Level::SerializeTileMap(Parser &aParser)
{
  aParser.Place("MapArtData", Common::IntVectorToString(mGenerator->GetArtTiles()));
  aParser.Place("Collision", Common::IntVectorToString(mGenerator->GetCollisionTiles()));
}

void Level::SerializeLUA()
{
  SLB::Class<Level>("Level")
          .set("Load", &Level::Load)
          .set("Unload", &Level::Unload)
          .set("GetName", &Level::GetName)
          .set("DeleteObject", &Level::DeleteObjectDelayed);
}

void Level::ParseFile()
{
  TextParser parser(Common::RelativePath("Game", mName).c_str());
  GameObject *object = NULL;
  HashString const curObject = "Object_";
  int curIndex = 0;

  HashString tempIndex = curObject + Common::IntToString(curIndex);
  Root* curRoot = parser.Find(tempIndex);

  while(curRoot)
  {
    // Make Object to assign params to
    ObjectManager *manager = mOwner->GetOwningApp()->GET<ObjectManager>();
    object = new GameObject(manager, curRoot->Find("File")->GetValue());
    manager->ParseObject(object);
    mObjects.push_back(object);

    if(curRoot->Find("Transform"))
    {
      Root* transform = curRoot->Find("Transform");
      int posX, posY, posZ,
          scaleX, scaleY, scaleZ,
          sizeX, sizeY, sizeZ;
      posX = transform->Find("PositionX")->GetValue().ToFloat();
      posY = transform->Find("PositionY")->GetValue().ToFloat();
      posZ = transform->Find("PositionZ")->GetValue().ToFloat();
      scaleX = transform->Find("ScaleX")->GetValue().ToFloat();
      scaleY = transform->Find("ScaleY")->GetValue().ToFloat();
      scaleZ = transform->Find("ScaleZ")->GetValue().ToFloat();
      sizeX = transform->Find("SizeX")->GetValue().ToFloat();
      sizeY = transform->Find("SizeY")->GetValue().ToFloat();
      sizeZ = transform->Find("SizeZ")->GetValue().ToFloat();

      Transform* objTransform = object->GET<Transform>();
      objTransform->SetPosition(Vector3(posX,posY,posZ));
      objTransform->SetScale(Vector3(scaleX,scaleY,scaleZ));
      objTransform->SetSize(Vector3(sizeX,sizeY,sizeZ));

      // Auto set camera bounds based on objects in environment
      mMinBoundary.x = Lesser<float>(posX - sizeX, mMinBoundary.x);
      mMinBoundary.y = Lesser<float>(posY - sizeY, mMinBoundary.x);
      mMaxBoundary.x = Greater<float>(posX + sizeX, mMaxBoundary.x);
      mMaxBoundary.y = Greater<float>(posY + sizeY, mMaxBoundary.x);
    }
    if(curRoot->Find("Surface"))
    {
      Root* surface = curRoot->Find("Surface");
      float r, g, b, a;
      r = surface->Find("ColorR")->GetValue().ToFloat();
      g = surface->Find("ColorG")->GetValue().ToFloat();
      b = surface->Find("ColorB")->GetValue().ToFloat();
      a = surface->Find("ColorA")->GetValue().ToFloat();

      Surface* objSurface = object->GET<Surface>();
      objSurface->SetColor(Vector4(r, g, b, a));
    }
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
    Root* tileMap = parser.Find("TileMapGenerator");
    HashString value, empty;
    int width, height, tileSize;
    HashString file, frameDataFilename, frameData,
                      collisionData;
    std::vector<int> frames, collision;

    width = tileMap->Find("Width")->GetValue().ToInt();
    height = tileMap->Find("Height")->GetValue().ToInt();
    tileSize = tileMap->Find("TileSize")->GetValue().ToInt();
    file = tileMap->Find("Image")->GetValue();
    frameDataFilename = tileMap->Find("Data")->GetValue();

    TextParser tileMapData(Common::RelativePath("Maps", frameDataFilename));
    frameData = tileMapData.Find("MapArtData")->GetValue();
    collisionData = tileMapData.Find("Collision")->GetValue();

    frames = Common::StringToIntVector(frameData);
    collision = Common::StringToIntVector(collisionData);

    mGenerator = new TileMapGenerator(width, height, tileSize,
                                     file, frameDataFilename,
                                     frames, collision, this);
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

Level::ObjectContainer& Level::GetObjects()
{
  return mObjects;
}
