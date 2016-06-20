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
                                   std::map<int, std::vector<int>> const &aAnimations,
                                   float const aAnimationSpeed, Level *aOwner) :
                                   mWidth(aWidth), mHeight(aHeight),
                                   mTileSize(aTileSize), mImageName(aImageName),
                                   mDataName(aDataName), mTiles(aTiles),
                                   mCollisionData(aCollisionData), mCollisionShapes(aCollisionShapes),
                                   mTileHeights(aTileHeights), mObjects(),
                                   mAnimationSpeed(aAnimationSpeed), mCurrentAnimationTime(0),
                                   mAnimatedObjects(), mAnimations(aAnimations), mCurrentFrames(),
                                   mOwner(aOwner)
{
  // mTiles and mCollisionData MUST be same size, or it's not a valid map.
  // For compatibility reasons, the shape data can be a different size because it can be empty.
  if(mTiles.size() != mCollisionData.size())
    assert(!"Not a valid tilemap, art and collision maps of different sizes");

  ObjectManager *objectManager = mOwner->GetManager()->GetOwningApp()->GET<ObjectManager>();
  PhysicsWorld *physicsWorld = mOwner->GetManager()->GetOwningApp()->GET<PhysicsWorld>();
  Vector3 tileSize = Vector3(mTileSize, mTileSize, 0);
  
  // Reserve total tiles ahead of time to avoid reallocs
  mObjects.reserve(mTiles.size());
  CreateTilesInRange(0, mTiles.size(), tileSize, objectManager, physicsWorld);
}

TileMapGenerator::~TileMapGenerator()
{
  mTiles.clear();
}

/**
  * @brief Update loop (i.e. update animations)
  */
void TileMapGenerator::Update()
{
  mCurrentAnimationTime += mOwner->GetManager()->GetOwningApp()->GetAppStep();

  if (mCurrentAnimationTime < mAnimationSpeed)
    return;
  mCurrentAnimationTime -= mAnimationSpeed;

  // Advance frames
  std::map<int, int>::iterator framesEnd = mCurrentFrames.end();
  for (std::map<int, int>::iterator it = mCurrentFrames.begin(); it != framesEnd; ++it)
  {
    int key = it->first;
    std::vector<int> animation = mAnimations.find(key)->second;
    int currentFrame = mCurrentFrames.find(key)->second;

    ++currentFrame;

    if(currentFrame >= static_cast<int>(animation.size()))
      currentFrame = 0;

    mCurrentFrames.find(key)->second = currentFrame;
  }

  // Set frames
  std::map<Surface*, int>::iterator animatedEnd = mAnimatedObjects.end();
  for (std::map<Surface*, int>::iterator it = mAnimatedObjects.begin(); it != animatedEnd; ++it)
  {
    std::vector<int> animation = mAnimations.find(it->second)->second;
    int currentFrame = mCurrentFrames.find(it->second)->second;

    it->first->SetFrameByID(animation[currentFrame]);
  }
}

/**
 * @brief Get Tilemap width
 * @return Width of tilemap
 */
int TileMapGenerator::GetWidth() const
{
  return mWidth;
}

/**
 * @brief Get Tilemap height
 * @return Height of tilemap
 */
int TileMapGenerator::GetHeight() const
{
  return mHeight;
}

/**
 * @brief Get tile size (in pixels)
 * @return Size of tiles in pixels.
 */
int TileMapGenerator::GetTileSize() const
{
  return mTileSize;
}

/**
 * @brief Get source image name
 * @return Name of source image
 */
std::string TileMapGenerator::GetImageName() const
{
  return mImageName;
}

/**
 * @brief Get data file name
 * @return Data file name
 */
std::string TileMapGenerator::GetDataName() const
{
  return mDataName;
}

/**
 * @brief Get art tiles vector
 * @return Art tiles vector
 */
std::vector<int>& TileMapGenerator::GetArtTiles()
{
  return mTiles;
}

/**
 * @brief Get collision tiles vector
 * @return Collision tiles vector
 */
std::vector<int>& TileMapGenerator::GetCollisionTiles()
{
  return mCollisionData;
}

/**
 * @brief Get collision shapes vector
 * @return Collision shapes vector
 */
std::vector<int>& TileMapGenerator::GetCollisionShapes()
{
  return mCollisionShapes;
}

/**
 * @brief Get tile height map
 * @return Tile height map
 */
std::map<int, float>& TileMapGenerator::GetTileHeights()
{
  return mTileHeights;
}

/**
 * @brief Get object at x, y index
 * @param aX X in world coordinates
 * @param aY Y in world coordinates
 * @return Object at position
 */
GameObject* TileMapGenerator::GetObject(int const aX, int const aY)
{
  return mObjects[GetIndex(aX, aY)];
}

/**
 * @brief Get object at index
 * @param aIndex Index to check
 * @return Object at index
 */
GameObject* TileMapGenerator::GetObject(int const aIndex)
{
  return mObjects[aIndex];
}

/**
 * @brief Get art value of tile at position
 * @param aX X in world coordinates
 * @param aY Y in world coordinates
 * @return Art value of tile at position
 */
int TileMapGenerator::GetTileValue(int const aX, int const aY)
{
  return mTiles[GetIndex(aX, aY)];
}

/**
 * @brief Get collision value of tile at position
 * @param aX X in world coordinates
 * @param aY Y in world coordinates
 * @return Collision value of tile at position
 */
int TileMapGenerator::GetCollisionValue(int const aX, int const aY)
{
  return mCollisionData[GetIndex(aX, aY)];
}

/**
 * @brief Helper method to get index value at position in world coordinates
 * @param aX X in world coordinates
 * @param aY Y in world coordinates
 * @return Index at position
 */
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

/**
 * @brief Helper method to get index values from position A to position B
 * @param aX aX in world coordinates
 * @param aY aY in world coordinates
 * @param bX bX in world coordinates
 * @param bY bY in world coordinates
 * @return Indices captured inside box drawn by coordinates.
 */
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

/**
 * @brief Serialize to file
 * @param aParser The file parser
 */
void TileMapGenerator::Serialize(Parser &aParser)
{
  aParser.Place("TileMapGenerator", "");
  aParser.Place("TileMapGenerator", "Width", Common::IntToString(mWidth));
  aParser.Place("TileMapGenerator", "Height", Common::IntToString(mHeight));
  aParser.Place("TileMapGenerator", "TileSize", Common::IntToString(mTileSize));
  aParser.Place("TileMapGenerator", "Image", mImageName);
  aParser.Place("TileMapGenerator", "Data", mDataName);
}

/**
 * @brief Helper method to create tiles from one index to another
 * @param aStart Index to start
 * @param aEnd Index to end
 * @param aTileSize Size of tiles in pixels
 * @param aObjectManager Object manager to add objects to
 * @param aPhysicsWorld Physics manager to add physics objects to
 */
void TileMapGenerator::CreateTilesInRange(unsigned const aStart, unsigned const aEnd, Vector3 const &aTileSize, ObjectManager *aObjectManager, PhysicsWorld *aPhysicsWorld)
{
  int xPos = aStart, yPos = 0;
  float halfX = mWidth * mTileSize;
  float halfY = mHeight * mTileSize;
  float const defaultZPos = -0.9999f;
  Vector3 const zeroVector = Vector3(0,0,0);
  unsigned const collisionDataVectorSize = mCollisionData.size();
  unsigned const tileDataVectorSize = mTiles.size();
  std::map<int, float>::const_iterator tileHeightsEnd = mTileHeights.end();
  PhysicsObject::IgnoreContainer ignoreContainer;
  
  // Make tiles ignore other tiles for collision
  for(int i = 0; i < 99; ++i)
  {
    HashString value = "Tile_" + Common::IntToString(i);
    ignoreContainer.insert(std::pair<int, HashString>(value.ToHash(), value));
  }
  
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
    aObjectManager->AddObject(obj, true);
    
    // Set name of tile, for collision reasons
    obj->SetName(std::string("Tile_") + Common::IntToString(mCollisionData[i]));

    // Get Transform of new object
    float zPos = defaultZPos;
    
    // If we have a height for this id, use it.
    if(mTileHeights.find(mTiles[i]) != tileHeightsEnd)
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
    else if(i == tileDataVectorSize - 1)
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
      if(mCollisionShapes.size() < collisionDataVectorSize ||
         mCollisionShapes[i] == CollisionShapes::CUBE ||
         mCollisionShapes[i] == CollisionShapes::CUBE_PASSABLE ||
         mCollisionShapes[i] >= CollisionShapes::ALL_COLLISION_SHAPES)
      {
        shape = new Cube(zeroVector, transform->GetSize());
        
        if(mCollisionShapes.size() < collisionDataVectorSize)
        {
          mCollisionShapes.push_back(CollisionShapes::CUBE);
        }
      }
      else if(mCollisionShapes[i] == CollisionShapes::TOPLEFT ||
              mCollisionShapes[i] == CollisionShapes::TOPLEFT_PASSABLE)
      {
        Vector3 point1(-triSize, -triSize, triZ);
        Vector3 point2(triSize, -triSize, triZ);
        Vector3 point3(-triSize, triSize, triZ);
        shape = new Triangle(point1, point2, point3);
      }
      else if(mCollisionShapes[i] == CollisionShapes::BOTTOMLEFT ||
              mCollisionShapes[i] == CollisionShapes::BOTTOMLEFT_PASSABLE)
      {
        Vector3 point1(-triSize, -triSize, triZ);
        Vector3 point2(triSize, triSize, triZ);
        Vector3 point3(-triSize, triSize, triZ);
        shape = new Triangle(point1, point2, point3);
      }
      else if(mCollisionShapes[i] == CollisionShapes::TOPRIGHT ||
              mCollisionShapes[i] == CollisionShapes::TOPRIGHT_PASSABLE)
      {
        Vector3 point1(-triSize, -triSize, triZ);
        Vector3 point2(triSize, -triSize, triZ);
        Vector3 point3(triSize, triSize, triZ);
        shape = new Triangle(point1, point2, point3);
      }
      else if(mCollisionShapes[i] == CollisionShapes::BOTTOMRIGHT ||
              mCollisionShapes[i] == CollisionShapes::BOTTOMRIGHT_PASSABLE)
      {
        Vector3 point1(triSize, -triSize, triZ);
        Vector3 point2(triSize, triSize, triZ);
        Vector3 point3(-triSize, triSize, triZ);
        shape = new Triangle(point1, point2, point3);
      }
      else if(mCollisionShapes[i] == CollisionShapes::BACKSLASH ||
              mCollisionShapes[i] == CollisionShapes::BACKSLASH_PASSABLE)
      {
        shape = new Line(Vector3(-triSize, -triSize, triZ), 
                         Vector3(triSize, triSize, triZ));
      }
      else if(mCollisionShapes[i] == CollisionShapes::FORWARDSLASH ||
              mCollisionShapes[i] == CollisionShapes::FORWARDSLASH_PASSABLE)
      {
        shape = new Line(Vector3(-triSize, triSize, triZ), 
                         Vector3(triSize, -triSize, triZ));
      }
      else if(mCollisionShapes[i] == CollisionShapes::LEFTSIDE ||
              mCollisionShapes[i] == CollisionShapes::LEFTSIDE_PASSABLE)
      {
        shape = new Line(Vector3(-triSize, -triSize, triZ), 
                         Vector3(-triSize, triSize, triZ));
      }
      else if(mCollisionShapes[i] == CollisionShapes::RIGHTSIDE ||
              mCollisionShapes[i] == CollisionShapes::RIGHTSIDE_PASSABLE)
      {
        shape = new Line(Vector3(triSize, triSize, triZ), 
                         Vector3(triSize, -triSize, triZ));
      }
      else if(mCollisionShapes[i] == CollisionShapes::TOPSIDE ||
              mCollisionShapes[i] == CollisionShapes::TOPSIDE_PASSABLE)
      {
        shape = new Line(Vector3(-triSize, -triSize, triZ), 
                         Vector3(triSize, -triSize, triZ));
      }
      else if(mCollisionShapes[i] == CollisionShapes::BOTTOMSIDE ||
              mCollisionShapes[i] == CollisionShapes::BOTTOMSIDE_PASSABLE)
      {
        shape = new Line(Vector3(-triSize, triSize, triZ), 
                         Vector3(triSize, triSize, triZ));
      }
      else if(mCollisionShapes[i] == CollisionShapes::HALFLEFT ||
              mCollisionShapes[i] == CollisionShapes::HALFLEFT_PASSABLE)
      {
        assert(!"Not supported");
      }
      else if(mCollisionShapes[i] == CollisionShapes::HALFRIGHT ||
              mCollisionShapes[i] == CollisionShapes::HALFRIGHT_PASSABLE)
      {
        assert(!"Not supported");
      }
      else if(mCollisionShapes[i] == CollisionShapes::HALFTOP ||
              mCollisionShapes[i] == CollisionShapes::HALFTOP_PASSABLE)
      {
        assert(!"Not supported");
      }
      else if(mCollisionShapes[i] == CollisionShapes::HALFBOTTOM ||
              mCollisionShapes[i] == CollisionShapes::HALFBOTTOM_PASSABLE)
      {
        assert(!"Not supported");
      }
      else if(mCollisionShapes[i] == CollisionShapes::QUARTERTOPLEFT ||
              mCollisionShapes[i] == CollisionShapes::QUARTERTOPLEFT_PASSABLE)
      {
        assert(!"Not supported");
      }
      else if(mCollisionShapes[i] == CollisionShapes::QUARTERTOPRIGHT ||
              mCollisionShapes[i] == CollisionShapes::QUARTERTOPRIGHT_PASSABLE)
      {
        assert(!"Not supported");
      }
      else if(mCollisionShapes[i] == CollisionShapes::QUARTERBOTTOMLEFT ||
              mCollisionShapes[i] == CollisionShapes::QUARTERBOTTOMLEFT_PASSABLE)
      {
        assert(!"Not supported");
      }
      else if(mCollisionShapes[i] == CollisionShapes::QUARTERBOTTOMRIGHT ||
              mCollisionShapes[i] == CollisionShapes::QUARTERBOTTOMRIGHT_PASSABLE)
      {
        assert(!"Not supported");
      }
      else
      {
        assert(!"Invalid value handed into TileMapGenerator.");
      }
      
      // Set passable if marked passable
      if(mCollisionShapes[i] >= CollisionShapes::CUBE_PASSABLE && 
         mCollisionShapes[i] < CollisionShapes::ALL_COLLISION_SHAPES)
      {
        physics->SetPassable(true);
      }
      
      // Finally, add shape to our physicsobject
      physics->AddShape(shape);
      // Set ignore list for physics
      physics->SetIgnoreList(ignoreContainer);

      obj->AddComponent(physics);
    }
    else if(mCollisionShapes.size() < collisionDataVectorSize)
    {
      mCollisionShapes.push_back(CollisionShapes::EMPTY);
    }

    // Animation
    if (mAnimations.find(mTiles[i]) != mAnimations.end())
    {
      mAnimatedObjects[surface] = mTiles[i];
      mCurrentFrames[mTiles[i]] = 0;
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