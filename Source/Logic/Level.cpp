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
#include "PlayerController.h"
#include "TileMapGenerator.h"
#include "Menu.h"
#include "Common.h"
#include "LuaIncludes.h"
#include "ObjectDeleteMessage.h"

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

void Level::AddMenu(Menu *aMenu)
{
  mMenus.push_back(aMenu);
}

void Level::RemoveMenu(Menu *aMenu)
{
  for(MenuIT it = mMenus.begin(); it != mMenus.end(); ++it)
  {
    if(*it == aMenu)
    {
      mMenus.erase(it);
      aMenu->DeleteObjects();
    }
  }
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
  mGenerator->Serialize(aParser);
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    (*it)->Serialize(aParser);
  }
  /*for(ObjectIT it = mStaticObjects.begin(); it != mStaticObjects.end(); ++it)
  {
    (*it)->Serialize(aParser);
  }*/
  // TODO focus target
  aParser.Place("Music", "");
  aParser.Place("Music", "Music", mMusicName);
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
          .set("GetName", &Level::GetName);
}

void Level::ParseFile()
{
	TextParser parser(Common::RelativePath("Game", mFileName).c_str(), false);
	GameObject *object = NULL;

	while(parser.IsGood())
	{
		std::string param;
		parser.GetNextString(param);

		if(param.length() == 0 || param == "}")
			break;

		if(param == "Transform")
		{
		  if(object)
		  {
		    Transform *transform = object->GET<Transform>();

		    float posX, posY, posZ,
		          scaleX, scaleY, scaleZ,
		          sizeX, sizeY, sizeZ;
        std::string empty;

        parser.GetNextFloat(posX);
        parser.GetNextFloat(posY);
        parser.GetNextFloat(posZ);
        parser.GetNextFloat(scaleX);
        parser.GetNextFloat(scaleY);
        parser.GetNextFloat(scaleZ);
        parser.GetNextFloat(sizeX);
        parser.GetNextFloat(sizeY);
        parser.GetNextFloat(sizeZ);

        transform->SetPosition(Vector3(posX,posY,posZ));
        transform->SetScale(Vector3(scaleX,scaleY,scaleZ));
        transform->SetSize(Vector3(sizeX,sizeY,sizeZ));

        // Auto set camera bounds based on objects in environment
        mMinBoundary.x = Lesser<float>(posX - sizeX, mMinBoundary.x);
        mMinBoundary.y = Lesser<float>(posY - sizeY, mMinBoundary.x);
        mMaxBoundary.x = Greater<float>(posX + sizeX, mMaxBoundary.x);
        mMaxBoundary.y = Greater<float>(posY + sizeY, mMaxBoundary.x);
		  }
		}
		else if(param == "Surface")
		{
		  float r, g, b, a;
		  parser.GetNextFloat(r);
		  parser.GetNextFloat(g);
		  parser.GetNextFloat(b);
		  parser.GetNextFloat(a);

		  Surface* surface = object->GET<Surface>();
		  surface->SetColor(Vector4(r, g, b, a));
		}
		else if(param == "Focus")
		{
		  std::string value, empty;

		  if(object)
		  {
        parser.GetNextString(empty);
        parser.GetNextString(value);
        parser.GetNextString(empty);

		    if(value == "true")
		    {
		      mOwner->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().SetTarget(object);
		      mFocusTarget = object;
		    }
		  }
		}
		else if(param == "PlayerController")
		{
		  PlayerController *controller = new PlayerController();
		  controller->SetTarget(object);

		  mOwner->GetOwningApp()->GET<ControllerManager>()->AddController(controller);
		  object->AddComponent(controller);
		}
    else if(param == "TileMapGenerator")
    {
      std::string value, empty;
      int width, height, tileSize;
      std::string file, frameDataFilename, frameData,
                  collisionData;
      std::vector<int> frames, collision;
      
      parser.GetNextInt(width);
      parser.GetNextInt(height);
      parser.GetNextInt(tileSize);
      parser.GetNextString(empty);
      parser.GetNextString(file);
      parser.GetNextString(empty);
      
      // Get the tilemap data (separate file)
      parser.GetNextString(frameDataFilename);
      
      TextParser tileMapData(Common::RelativePath("Maps", frameDataFilename), false);
      tileMapData.GetNextString(empty);
      tileMapData.GetNextString(frameData);
      tileMapData.GetNextString(empty);
      tileMapData.GetNextString(collisionData);
      
      frames = Common::StringToIntVector(frameData);
      collision = Common::StringToIntVector(collisionData);
      
      mGenerator = new TileMapGenerator(width, height, tileSize,
                                       file, frameDataFilename,
                                       frames, collision, this);
    }
    else if(param == "Music")
    {
      std::string empty, music;
      parser.GetNextString(music);

      mMusicName = music;
    }
    else if(param == "Bounds")
    {
      /*
       * Set camera bounds manually
       * 3 places where bounds can be set:
       * 1. Auto set from object transforms, see above
       * 2. Calculated from tilemap, see TileMapGenerator.cpp
       * 3. See below
       */
      std::string empty;
      int x, y;
      parser.GetNextInt(x);
      parser.GetNextInt(y);
      mMaxBoundary = Vector3(x, y, 0);
      parser.GetNextInt(x);
      parser.GetNextInt(y);
      mMinBoundary = Vector3(x, y, 0);
    }
		else if(param[param.size() - 4] == '.')
		{
      ObjectManager *manager = mOwner->GetOwningApp()->GET<ObjectManager>();
		  object = new GameObject(manager, param);
		  manager->ParseObject(object);
		  mObjects.push_back(object);
		}
		else
		{
		  ParseAdditionalData(&parser, object, param);
		}
	}
}
