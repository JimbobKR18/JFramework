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
#include "AutoCrit.h"

TileMapGenerator::TileMapGenerator()
{
  assert(0);
}

TileMapGenerator::TileMapGenerator(int aWidth, int aHeight, int aTileSize,
                                   std::string const &aImageName,
                                   std::string const &aDataName,
                                   std::vector<int> const &aTiles,
                                   std::vector<int> const &aCollisionData,
                                   std::vector<int> const &aCollisionShapes,
                                   std::map<int, float> const &aTileHeights,
                                   Level *aOwner) :
                                   mWidth(aWidth), mHeight(aHeight),
                                   mTileSize(aTileSize), mImageName(aImageName),
                                   mDataName(aDataName), mTiles(aTiles),
                                   mCollisionData(aCollisionData), mCollisionShapes(aCollisionShapes),
                                   mTileHeights(aTileHeights), mObjects(), mOwner(aOwner)
{
  // mTiles and mCollisionData MUST be same size, or it's not a valid map.
  // For compatibility reasons, the shape data can be a different size because it can be empty.
  if(mTiles.size() != mCollisionData.size())
    assert(!"Not a valid tilemap, art and collision maps of different sizes");

  ObjectManager *objectManager = mOwner->GetManager()->GetOwningApp()->GET<ObjectManager>();
  PhysicsWorld *physicsWorld = mOwner->GetManager()->GetOwningApp()->GET<PhysicsWorld>();
  Vector3 tileSize = Vector3(mTileSize, mTileSize, 0);
  CreateTilesInRange(0, mTiles.size(), tileSize, objectManager, physicsWorld);
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
  return mCollisionData;
}

std::vector<int>& TileMapGenerator::GetCollisionShapes()
{
  return mCollisionShapes;
}

std::map<int, float>& TileMapGenerator::GetTileHeights()
{
  return mTileHeights;
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
  return mCollisionData[GetIndex(aX, aY)];
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

void TileMapGenerator::CreateTilesInRange(unsigned const aStart, unsigned const aEnd, Vector3 const &aTileSize, ObjectManager *aObjectManager, PhysicsWorld *aPhysicsWorld)
{
  int xPos = aStart, yPos = 0;
  float halfX = mWidth * mTileSize;
  float halfY = mHeight * mTileSize;
  float const defaultZPos = -0.9999f;
  
  // Get the position of the starting index.
  while(xPos >= mWidth)
  {
    ++yPos;
    xPos -= mWidth;
  }
  
  for(unsigned int i = aStart; i != aEnd; ++i)
  {
    // Make GameObject to place
    GameObject *obj = new GameObject(aObjectManager, mImageName);
    aObjectManager->ParseObject(obj);
    
    // Set name of tile, for collision reasons
    obj->SetName(std::string("Tile_") + Common::IntToString(mCollisionData[i]));

    // Get Transform of new object
    float zPos = defaultZPos;
    
    // If we have a height for this id, use it.
    if(mTileHeights.find(mTiles[i]) != mTileHeights.end())
    {
      zPos = mTileHeights[mTiles[i]];
    }
    
    Transform *transform = obj->GET<Transform>();
    Vector3 position = Vector3(-halfX + (mTileSize * 2 * xPos),
                                                  -halfY + (mTileSize * 2 * yPos), zPos);
    transform->SetPosition(position);
    transform->SetSize(aTileSize);
    
    // Figure out the max and min camera boundaries based on tilemap
    if(i == 0)
      mOwner->SetMinBoundary(position - aTileSize);
    else if(i == mTiles.size() - 1)
      mOwner->SetMaxBoundary(position + aTileSize);

    // Set the frame data
    Surface *surface = obj->GET<Surface>();
    surface->SetAnimated(false);
    surface->SetFrameByID(mTiles[i]);
    
    // Add PhysicsObject if the tile has collision
    if(mCollisionData[i] != CollisionShapes::EMPTY ||
        (mCollisionShapes.size() > i && mCollisionShapes[i] != CollisionShapes::EMPTY))
    {
      PhysicsObject *physics = aPhysicsWorld->CreateObject();
      
      // What shape is our object? Is it affected by gravity?
      // What is the object's mass? Is it static?
      aPhysicsWorld->UnregisterGravity(physics);
      physics->SetMass(50);
      physics->SetStatic(true);
      physics->SetBroadSize(transform->GetSize() * 1.5f);
      
      // Based on the shape passed in from file
      Shape *shape = nullptr;
      float triSize = mTileSize;
      float triZ = -zPos;
      if(mCollisionShapes.size() < mCollisionData.size() ||
         mCollisionShapes[i] == CollisionShapes::CUBE ||
         mCollisionShapes[i] > CollisionShapes::BOTTOMRIGHT)
      {
        shape = new Cube(Vector3(0,0,0), Vector3(transform->GetSize()));
        
        if(mCollisionShapes.size() < mCollisionData.size())
        {
          mCollisionShapes.push_back(CollisionShapes::CUBE);
        }
      }
      else if(mCollisionShapes[i] == CollisionShapes::TOPLEFT)
      {
        Vector3 point1(-triSize, -triSize, triZ);
        Vector3 point2(triSize, -triSize, triZ);
        Vector3 point3(-triSize, triSize, triZ);
        shape = new Triangle(point1, point2, point3);
      }
      else if(mCollisionShapes[i] == CollisionShapes::BOTTOMLEFT)
      {
        Vector3 point1(-triSize, -triSize, triZ);
        Vector3 point2(triSize, triSize, triZ);
        Vector3 point3(-triSize, triSize, triZ);
        shape = new Triangle(point1, point2, point3);
      }
      else if(mCollisionShapes[i] == CollisionShapes::TOPRIGHT)
      {
        Vector3 point1(-triSize, -triSize, triZ);
        Vector3 point2(triSize, -triSize, triZ);
        Vector3 point3(triSize, triSize, triZ);
        shape = new Triangle(point1, point2, point3);
      }
      else if(mCollisionShapes[i] == CollisionShapes::BOTTOMRIGHT)
      {
        Vector3 point1(triSize, -triSize, triZ);
        Vector3 point2(triSize, triSize, triZ);
        Vector3 point3(-triSize, triSize, triZ);
        shape = new Triangle(point1, point2, point3);
      }
      else
      {
        assert(!"Invalid value handed into TileMapGenerator.");
      }
      
      // Finally, add shape to our physicsobject
      physics->AddShape(shape);
      
      // Make tiles ignore other tiles for collision
      for(int i = 0; i < 99; ++i)
      {
        char buffer[33];
        sprintf(buffer, "Tile_%d", i);
        physics->AddIgnore(std::string(buffer));
      }

      obj->AddComponent(physics);
    }
    else if(mCollisionShapes.size() < mCollisionData.size())
    {
      mCollisionShapes.push_back(CollisionShapes::EMPTY);
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