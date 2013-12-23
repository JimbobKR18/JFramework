//
//  TileMapGenerator.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/31/12.
//
//

#include "TileMapGenerator.h"
#include "ObjectManager.h"
#include "LevelManager.h"
#include "Transform.h"
#include "Surface.h"
#include "PhysicsObject.h"

TileMapGenerator::TileMapGenerator()
{
  assert(0);
}

TileMapGenerator::TileMapGenerator(int aWidth, int aHeight, int aTileSize,
                                   std::string const &aFilename,
                                   std::vector<int> const &aTiles,
                                   std::vector<int> const &aCollision,
                                   Level *aOwner) :
                                   mWidth(aWidth), mHeight(aHeight),
                                   mTileSize(aTileSize), mFilename(aFilename),
                                   mTiles(aTiles), mCollision(aCollision),
                                   mOwner(aOwner)
{
  int xPos = 0, yPos = 0;
  float halfX = mWidth * aTileSize;
  float halfY = mHeight * aTileSize;
  // mTiles and mCollision MUST be same size, or it's not a valid map
  
  if(mTiles.size() != mCollision.size())
    assert(!"Not a valid tilemap, art and collision maps of different sizes");
  
  for(unsigned int i = 0; i != mTiles.size(); ++i)
  {
    // Make GameObject to place
    ObjectManager *manager = mOwner->GetManager()->GetOwningApp()->GET<ObjectManager>();
    GameObject *obj = new GameObject(manager, mFilename);
    manager->ParseObject(obj);
    
    // Set name of tile, for collision reasons
    obj->SetName(std::string("Tile_") + Common::IntToString(mCollision[i]));

    // Get Transform of new object
    Transform *transform = obj->GET<Transform>();
    transform->SetPosition(Vector3(-halfX + (aTileSize * 2 * xPos),
                                   -halfY + (aTileSize * 2 * yPos),0));
    transform->SetSize(Vector3(mTileSize, mTileSize, 0));
    
    // Set the frame data
    Surface *surface = obj->GET<Surface>();
    surface->SetAnimated(false);
    surface->GetTextureData()->SetCurrentFrame(mTiles[i]);
    
    // Add PhysicsObject if the tile has collision
    if(mCollision[i] != 0)
    {
      PhysicsObject *physics = mOwner->GetManager()->GetOwningApp()->GET<PhysicsWorld>()->CreateObject();
      
      // What shape is our object? Is it affected by gravity?
      // What is the object's mass? Is it static?
      physics->SetMass(50);
      mOwner->GetManager()->GetOwningApp()->GET<PhysicsWorld>()->UnregisterGravity(physics);
      physics->SetStatic(true);
      physics->mShape = PhysicsObject::CUBE;
      
      // Make tiles ignore other tiles for collision
      for(int i = 0; i < 99; ++i)
      {
        char buffer[33];
        sprintf(buffer, "Tile_%d", i);
        physics->AddIgnore(std::string(buffer));
      }

      obj->AddComponent(physics);
    }
    
    // Add object to our level for easier loading later
    mOwner->AddStaticObject(obj);
    
    ++xPos;
    if(xPos >= mWidth)
    {
      xPos = 0;
      ++yPos;
    }
  }
}

TileMapGenerator::~TileMapGenerator()
{
  mTiles.clear();
}
