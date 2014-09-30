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
#include "Level.h"

TileMapGenerator::TileMapGenerator()
{
  assert(0);
}

TileMapGenerator::TileMapGenerator(int aWidth, int aHeight, int aTileSize,
                                   std::string const &aImageName,
                                   std::string const &aDataName,
                                   std::vector<int> const &aTiles,
                                   std::vector<int> const &aCollision,
                                   Level *aOwner) :
                                   mWidth(aWidth), mHeight(aHeight),
                                   mTileSize(aTileSize), mImageName(aImageName),
                                   mDataName(aDataName), mTiles(aTiles),
                                   mCollision(aCollision), mObjects(), mOwner(aOwner)
{
  int xPos = 0, yPos = 0;
  float halfX = mWidth * aTileSize;
  float halfY = mHeight * aTileSize;
  float const zPos = -0.9999f;

  // mTiles and mCollision MUST be same size, or it's not a valid map
  if(mTiles.size() != mCollision.size())
    assert(!"Not a valid tilemap, art and collision maps of different sizes");

  Vector3 tileSize = Vector3(mTileSize, mTileSize, 0);
  for(unsigned int i = 0; i != mTiles.size(); ++i)
  {
    // Make GameObject to place
    ObjectManager *manager = mOwner->GetManager()->GetOwningApp()->GET<ObjectManager>();
    GameObject *obj = new GameObject(manager, mImageName);
    manager->ParseObject(obj);
    
    // Set name of tile, for collision reasons
    obj->SetName(std::string("Tile_") + Common::IntToString(mCollision[i]));

    // Get Transform of new object
    Transform *transform = obj->GET<Transform>();
    Vector3 position = Vector3(-halfX + (aTileSize * 2 * xPos),
                               -halfY + (aTileSize * 2 * yPos), zPos);
    transform->SetPosition(position);
    transform->SetSize(tileSize);
    
    // Figure out the max and min camera boundaries based on tilemap
    if(i == 0)
      mOwner->SetMinBoundary(position - tileSize);
    else if(i == mTiles.size() - 1)
      mOwner->SetMaxBoundary(position + tileSize);

    // Set the frame data
    Surface *surface = obj->GET<Surface>();
    surface->SetAnimated(false);
    surface->SetFrameByID(mTiles[i]);
    
    // Add PhysicsObject if the tile has collision
    if(mCollision[i] != 0)
    {
      PhysicsObject *physics = mOwner->GetManager()->GetOwningApp()->GET<PhysicsWorld>()->CreateObject();
      
      // What shape is our object? Is it affected by gravity?
      // What is the object's mass? Is it static?
      physics->SetMass(50);
      mOwner->GetManager()->GetOwningApp()->GET<PhysicsWorld>()->UnregisterGravity(physics);
      physics->SetStatic(true);
      physics->SetBroadSize(transform->GetSize() * 1.5f);
      
      physics->AddShape(new Cube(Vector3(0,0,0),
                                 Vector3(transform->GetSize())));
      
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
    
    // Add object to our tile list
    mObjects.push_back(obj);

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

int TileMapGenerator::GetWidth() const
{
  return mWidth;
}

int TileMapGenerator::GetHeight() const
{
  return mHeight;
}

int TileMapGenerator::GetTileSize() const
{
  return mTileSize;
}

std::string TileMapGenerator::GetImageName() const
{
  return mImageName;
}

std::string TileMapGenerator::GetDataName() const
{
  return mDataName;
}

std::vector<int>& TileMapGenerator::GetArtTiles()
{
  return mTiles;
}

std::vector<int>& TileMapGenerator::GetCollisionTiles()
{
  return mCollision;
}

GameObject* TileMapGenerator::GetObject(int const aX, int const aY)
{
  return mObjects[GetIndex(aX, aY)];
}

GameObject* TileMapGenerator::GetObject(int const aIndex)
{
  return mObjects[aIndex];
}

int TileMapGenerator::GetTileValue(int const aX, int const aY)
{
  return mTiles[GetIndex(aX, aY)];
}

int TileMapGenerator::GetCollisionValue(int const aX, int const aY)
{
  return mCollision[GetIndex(aX, aY)];
}

int TileMapGenerator::GetIndex(int const aX, int const aY)
{
  int tileMult = mTileSize;
  float x = mWidth * tileMult;
  float y = mHeight * tileMult;
  x += aX;
  y += aY;
  x /= mTileSize * 2.0f;
  y /= mTileSize * 2.0f;

  if(x < 0)
    x = 0;
  if(y < 0)
    y = 0;

  return (round(y) * mWidth) + round(x);
}

std::vector<int> TileMapGenerator::GetIndices(int const aX, int const aY, int const bX, int const bY)
{
  std::vector<int> ret;
  int y = aY;
  int x = aX;
  int xDir = (aX < bX) ? 1 : -1;
  int yDir = (aY < bY) ? 1 : -1;
  int step = mTileSize * 2.0f;
  int xStep = step * xDir;
  int yStep = step * yDir;
  int xHalfStep = xStep / 2.0f;
  int yHalfStep = yStep / 2.0f;
  int xEnd = ((bX + xHalfStep) - aX) / xStep;
  int yEnd = ((bY + yHalfStep) - aY) / yStep;
  for(int yCur = 0; yCur <= yEnd; y += yStep, ++yCur)
  {
    x = aX;
    for(int xCur = 0; xCur <= xEnd; x += xStep, ++xCur)
    {
      ret.push_back(GetIndex(x, y));
    }
  }
  return ret;
}

void TileMapGenerator::Serialize(Parser &aParser)
{
  aParser.Place("TileMapGenerator", "");
  aParser.Place("TileMapGenerator", "Width", Common::IntToString(mWidth));
  aParser.Place("TileMapGenerator", "Height", Common::IntToString(mHeight));
  aParser.Place("TileMapGenerator", "TileSize", Common::IntToString(mTileSize));
  aParser.Place("TileMapGenerator", "Image", mImageName);
  aParser.Place("TileMapGenerator", "Data", mDataName);
}

