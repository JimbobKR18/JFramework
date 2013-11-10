#include "Level.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "MathExt.h"
#include "Transform.h"
#include "GraphicsManager.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "ControllerManager.h"
#include "TileMapGenerator.h"
#include "Menu.h"
#include "Common.h"
#include "LuaIncludes.h"

Level::Level()
{
	assert(0);
}

Level::Level(LevelManager *aManager, std::string const &aFileName) :
             mName(""), mFileName(aFileName), mObjects(),
             mMenus(), mOwner(aManager), mActive(false)
{
	for(int i = static_cast<int>(aFileName.size()) - 1;
      aFileName[i] != '/' && i >= 0; --i)
	{
		mName.push_back(aFileName[i]);
	}
	std::reverse(mName.begin(), mName.end());

	ParseFile();
}

Level::~Level()
{
	if(mActive)
		mObjects.clear();
	else
    DeleteObjects();
}

std::string Level::GetName() const
{
	return mName;
}

LevelManager *Level::GetManager() const
{
  return mOwner;
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

void Level::DeleteObject(GameObject *aObject)
{
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    if(aObject == *it)
    {
      mObjects.erase(it);
      ObjectManager *manager = mOwner->GetOwningApp()->GET<ObjectManager>();
      manager->DeleteObject(aObject);
      break;
    }
  }
}

void Level::DeleteObjects()
{
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    ObjectManager *manager = mOwner->GetOwningApp()->GET<ObjectManager>();
    manager->DeleteObject(*it);
  }
  mObjects.clear();
}

void Level::Reset()
{
  DeleteObjects();
  ParseFile();
}

void Level::Load()
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
	mActive = true;
  
  mOwner->SetActiveLevel(this);
}

void Level::Unload()
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
	mActive = false;
  
  mOwner->SetActiveLevel(NULL);
}

void Level::SerializeLUA()
{
  SLB::Class<Level>("Level").constructor()
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
		  }
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
		    }
		  }
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
      
      TileMapGenerator tilemap(width, height, tileSize,
                               file, frames, collision, this);
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
		  ParseAdditionalData(&parser);
		}
	}
}
