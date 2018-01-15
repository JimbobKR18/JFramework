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
#include "ChemistryMaterial.h"
#include "Transform.h"
#include "Surface.h"
#include "PhysicsObject.h"
#include "Level.h"
#include "Threading.h"

//---------------------------------------------
// TileMapGenerator
//---------------------------------------------
TileMapGenerator::TileMapGenerator()
{
  assert(0);
}

TileMapGenerator::TileMapGenerator(int aWidth, int aHeight, int aTileSize,
                                   HashString const &aImageName,
                                   HashString const &aDataName,
                                   std::vector<int> const &aTiles,
                                   std::vector<int> const &aCollisionData,
                                   std::vector<int> const &aCollisionShapes,
                                   std::vector<int> const &aMaterials,
                                   std::unordered_map<int, float> const &aTileHeights,
                                   std::unordered_map<int, HashString> const &aMaterialNames,
                                   std::unordered_map<int, std::vector<int>> const &aAnimations,
                                   float const aAnimationSpeed, Level *aOwner) :
                                   mWidth(aWidth), mHeight(aHeight),
                                   mTileSize(aTileSize), mImageName(aImageName),
                                   mDataName(aDataName), mTiles(aTiles),
                                   mCollisionData(aCollisionData), mCollisionShapes(aCollisionShapes),
                                   mTileHeights(aTileHeights), mObjects(),
                                   mAnimationSpeed(aAnimationSpeed), mCurrentAnimationTime(0),
                                   mAnimatedObjects(), mAnimations(aAnimations), mCurrentFrames(),
                                   mMaterials(aMaterials), mMaterialNames(aMaterialNames),
                                   mOwner(aOwner)
{
  // mTiles and mCollisionData MUST be same size, or it's not a valid map.
  // For compatibility reasons, the shape data can be a different size because it can be empty.
  if(mTiles.size() != mCollisionData.size())
    assert(!"Not a valid tilemap, art and collision maps of different sizes");

  ObjectManager *objectManager = mOwner->GetManager()->GetOwningApp()->GET<ObjectManager>();
  PhysicsWorld *physicsWorld = mOwner->GetManager()->GetOwningApp()->GET<PhysicsWorld>();
  ChemistryManager *chemistryManager = mOwner->GetManager()->GetOwningApp()->GET<ChemistryManager>();
  Vector3 tileSize = Vector3(mTileSize, mTileSize, mTileSize);
  
  // Reserve total tiles ahead of time to avoid reallocs
  mObjects.resize(mTiles.size(), nullptr);
  CreateTilesInRange(0, 0, mWidth, mHeight, tileSize, objectManager, physicsWorld, chemistryManager);
}

TileMapGenerator::~TileMapGenerator()
{
  mTiles.clear();
  mCollisionData.clear();
  mCollisionShapes.clear();
  mTileHeights.clear();
  mMaterials.clear();
  mMaterialNames.clear();
  mAnimations.clear();
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
  std::unordered_map<int, int>::iterator framesEnd = mCurrentFrames.end();
  for (std::unordered_map<int, int>::iterator it = mCurrentFrames.begin(); it != framesEnd; ++it)
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
  std::unordered_map<Surface*, int>::iterator animatedEnd = mAnimatedObjects.end();
  for (std::unordered_map<Surface*, int>::iterator it = mAnimatedObjects.begin(); it != animatedEnd; ++it)
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
HashString TileMapGenerator::GetImageName() const
{
  return mImageName;
}

/**
 * @brief Get data file name
 * @return Data file name
 */
HashString TileMapGenerator::GetDataName() const
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
std::unordered_map<int, float>& TileMapGenerator::GetTileHeights()
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
 * @param aNode ParserNode to write to.
 */
void TileMapGenerator::Serialize(ParserNode *aNode)
{
  aNode->Place("TileMapGenerator", "");
  
  ParserNode *tileMapNode = aNode->Find("TileMapGenerator");
  tileMapNode->Place("Width", Common::IntToString(mWidth));
  tileMapNode->Place("Height", Common::IntToString(mHeight));
  tileMapNode->Place("TileSize", Common::IntToString(mTileSize));
  tileMapNode->Place("Image", mImageName);
  tileMapNode->Place("Data", mDataName);
}

/**
 * @brief Helper method to create tiles from one index to another
 * @param aXStart X Index to start
 * @param aYStart Y Index to start
 * @param aXEnd X Index to end
 * @param aYEnd Y Index to end
 * @param aTileSize Size of tiles in pixels
 * @param aObjectManager Object manager to add objects to
 * @param aPhysicsWorld Physics manager to add physics objects to
 * @param aChemistryManager Chemistry manager to create materials and such
 */
void TileMapGenerator::CreateTilesInRange(unsigned const aXStart, unsigned const aYStart, 
  unsigned const aXEnd, unsigned const aYEnd, Vector3 const &aTileSize, 
  ObjectManager *aObjectManager, PhysicsWorld *aPhysicsWorld, ChemistryManager *aChemistryManager)
{
  float halfX = mWidth * mTileSize;
  float halfY = mHeight * mTileSize;
  float const defaultZPos = -0.9999f;
  Vector3 const zeroVector = Vector3(0,0,0);
  unsigned const collisionDataVectorSize = mCollisionData.size();
  unsigned const tileDataVectorSize = mTiles.size();
  std::unordered_map<int, float>::const_iterator tileHeightsEnd = mTileHeights.end();
  PhysicsObject::IgnoreContainer ignoreContainer;
  
  // Bounds checking
  if(aXEnd < aXStart || aYEnd < aYStart)
    assert(!"ERROR [TileMapGenerator]: Ends cannot be before starts.");
  else if(aXStart < 0 || aYStart < 0)
    assert(!"ERROR [TileMapGenerator]: Starts cannot be less than 0.");
  else if(aXEnd > mWidth || aYEnd > mHeight)
    assert(!"ERROR [TileMapGenerator]: Ends cannot exceed bounds of map.");
  
  for(unsigned int y = aYStart; y < aYEnd; ++y)
  {
    for(unsigned int x = aXStart; x < aXEnd; ++x)
    {
      // Calculate index
      int i = x + (y * mWidth);
      
      // Basic check to be sure we're not overwriting an index
      if(mObjects[i] != nullptr)
        continue;
      
      // Make GameObject to place
      GameObject *obj = aObjectManager->CreateObjectNoAdd(mImageName);
      aObjectManager->AddObject(obj, true);
      
      // Set name of tile, for collision reasons
      obj->SetName(HashString("Tile_") + Common::IntToString(mCollisionData[i]));

      // Get Transform of new object
      float zPos = defaultZPos;
      
      // If we have a height for this id, use it.
      if(mTileHeights.find(mTiles[i]) != tileHeightsEnd)
      {
        zPos = mTileHeights[mTiles[i]];
      }
      
      Transform *transform = obj->GET<Transform>();
      Vector3 position = Vector3(-halfX + (mTileSize * 2 * x),
                                 -halfY + (mTileSize * 2 * y), zPos);
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
        PhysicsObject *physics = CreatePhysicsAtIndex(i, aPhysicsWorld, transform, 
          zeroVector, collisionDataVectorSize, zPos);
        
        // Set ignore list for physics
        physics->SetIgnoreList(ignoreContainer);

        obj->AddComponent(physics);
      }
      else if(mCollisionShapes.size() < collisionDataVectorSize)
      {
        mCollisionShapes.push_back(CollisionShapes::EMPTY);
      }

      // Animation
      if(mAnimations.find(mTiles[i]) != mAnimations.end())
      {
        mAnimatedObjects[surface] = mTiles[i];
        mCurrentFrames[mTiles[i]] = 0;
      }
      
      // Materials, if data is available
      if(mMaterials.size() > 0 && mMaterialNames.find(mMaterials[i]) != mMaterialNames.end())
      {
        ChemistryMaterial *material = CreateMaterialAtIndex(i, aChemistryManager);
        obj->AddComponent(material);
      }
      
      // Add object to our level for easier loading later
      mOwner->AddObject(obj, ObjectPlacement::STATIC);
      
      // Add object to our tile list
      mObjects[i] = obj;
    }
  }
}

/**
 * @brief Create physics object at index
 * @param aIndex Index to create physics at.
 * @param aPhysicsWorld Physics manager.
 * @param aTransform Transform of object.
 * @param aZeroVector Empty vector.
 * @param aCollisionDataVectorSize Size of collision data vector.
 * @param aZPos Z position of shape.
 * @return new Physics object.
 */
PhysicsObject* TileMapGenerator::CreatePhysicsAtIndex(unsigned const aIndex, PhysicsWorld *aPhysicsWorld, 
  Transform *aTransform, Vector3 const &aZeroVector, unsigned const aCollisionDataVectorSize, float aZPos)
{
  PhysicsObject *physics = aPhysicsWorld->CreateObject();
      
  // What shape is our object? Is it affected by gravity?
  // What is the object's mass? Is it static?
  aPhysicsWorld->UnregisterGravity(physics);
  physics->SetMass(FLT_MAX);
  physics->SetStatic(true);
  physics->SetBroadSize(aTransform->GetSize() * 1.5f);
  
  // Based on the shape passed in from file
  Shape *shape = nullptr;
  float triSize = mTileSize;
  float triZ = -aZPos;
  if(mCollisionShapes.size() < aCollisionDataVectorSize ||
     mCollisionShapes[aIndex] == CollisionShapes::CUBE ||
     mCollisionShapes[aIndex] == CollisionShapes::CUBE_PASSABLE ||
     mCollisionShapes[aIndex] >= CollisionShapes::ALL_COLLISION_SHAPES)
  {
    shape = new AxisAlignedBoundingBox(aZeroVector, aTransform->GetSize());
    
    if(mCollisionShapes.size() < aCollisionDataVectorSize)
    {
      mCollisionShapes.push_back(CollisionShapes::CUBE);
    }
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::TOPLEFT ||
          mCollisionShapes[aIndex] == CollisionShapes::TOPLEFT_PASSABLE)
  {
    Vector3 point1(-triSize, -triSize, triZ);
    Vector3 point2(triSize, -triSize, triZ);
    Vector3 point3(-triSize, triSize, triZ);
    shape = new Triangle(point1, point2, point3);
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::BOTTOMLEFT ||
          mCollisionShapes[aIndex] == CollisionShapes::BOTTOMLEFT_PASSABLE)
  {
    Vector3 point1(-triSize, -triSize, triZ);
    Vector3 point2(triSize, triSize, triZ);
    Vector3 point3(-triSize, triSize, triZ);
    shape = new Triangle(point1, point2, point3);
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::TOPRIGHT ||
          mCollisionShapes[aIndex] == CollisionShapes::TOPRIGHT_PASSABLE)
  {
    Vector3 point1(-triSize, -triSize, triZ);
    Vector3 point2(triSize, -triSize, triZ);
    Vector3 point3(triSize, triSize, triZ);
    shape = new Triangle(point1, point2, point3);
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::BOTTOMRIGHT ||
          mCollisionShapes[aIndex] == CollisionShapes::BOTTOMRIGHT_PASSABLE)
  {
    Vector3 point1(triSize, -triSize, triZ);
    Vector3 point2(triSize, triSize, triZ);
    Vector3 point3(-triSize, triSize, triZ);
    shape = new Triangle(point1, point2, point3);
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::BACKSLASH ||
          mCollisionShapes[aIndex] == CollisionShapes::BACKSLASH_PASSABLE)
  {
    shape = new Line(Vector3(-triSize, -triSize, triZ), 
                     Vector3(triSize, triSize, triZ));
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::FORWARDSLASH ||
          mCollisionShapes[aIndex] == CollisionShapes::FORWARDSLASH_PASSABLE)
  {
    shape = new Line(Vector3(-triSize, triSize, triZ), 
                     Vector3(triSize, -triSize, triZ));
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::LEFTSIDE ||
          mCollisionShapes[aIndex] == CollisionShapes::LEFTSIDE_PASSABLE)
  {
    shape = new Line(Vector3(-triSize, -triSize, triZ), 
                     Vector3(-triSize, triSize, triZ));
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::RIGHTSIDE ||
          mCollisionShapes[aIndex] == CollisionShapes::RIGHTSIDE_PASSABLE)
  {
    shape = new Line(Vector3(triSize, triSize, triZ), 
                     Vector3(triSize, -triSize, triZ));
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::TOPSIDE ||
          mCollisionShapes[aIndex] == CollisionShapes::TOPSIDE_PASSABLE)
  {
    shape = new Line(Vector3(-triSize, -triSize, triZ), 
                     Vector3(triSize, -triSize, triZ));
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::BOTTOMSIDE ||
          mCollisionShapes[aIndex] == CollisionShapes::BOTTOMSIDE_PASSABLE)
  {
    shape = new Line(Vector3(-triSize, triSize, triZ), 
                     Vector3(triSize, triSize, triZ));
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::HALFLEFT ||
          mCollisionShapes[aIndex] == CollisionShapes::HALFLEFT_PASSABLE)
  {
    assert(!"Not supported");
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::HALFRIGHT ||
          mCollisionShapes[aIndex] == CollisionShapes::HALFRIGHT_PASSABLE)
  {
    assert(!"Not supported");
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::HALFTOP ||
          mCollisionShapes[aIndex] == CollisionShapes::HALFTOP_PASSABLE)
  {
    assert(!"Not supported");
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::HALFBOTTOM ||
          mCollisionShapes[aIndex] == CollisionShapes::HALFBOTTOM_PASSABLE)
  {
    assert(!"Not supported");
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::QUARTERTOPLEFT ||
          mCollisionShapes[aIndex] == CollisionShapes::QUARTERTOPLEFT_PASSABLE)
  {
    assert(!"Not supported");
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::QUARTERTOPRIGHT ||
          mCollisionShapes[aIndex] == CollisionShapes::QUARTERTOPRIGHT_PASSABLE)
  {
    assert(!"Not supported");
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::QUARTERBOTTOMLEFT ||
          mCollisionShapes[aIndex] == CollisionShapes::QUARTERBOTTOMLEFT_PASSABLE)
  {
    assert(!"Not supported");
  }
  else if(mCollisionShapes[aIndex] == CollisionShapes::QUARTERBOTTOMRIGHT ||
          mCollisionShapes[aIndex] == CollisionShapes::QUARTERBOTTOMRIGHT_PASSABLE)
  {
    assert(!"Not supported");
  }
  else
  {
    DebugLogPrint("ERROR [TileMapGenerator]: Index %d has Art %d, Collision ID %d, and Shape %d", aIndex, 
      mTiles[aIndex], mCollisionData[aIndex], mCollisionShapes[aIndex]);
    assert(!"Invalid value handed into TileMapGenerator.");
  }
  
  // Set passable if marked passable
  if(mCollisionShapes[aIndex] >= CollisionShapes::CUBE_PASSABLE && 
     mCollisionShapes[aIndex] < CollisionShapes::ALL_COLLISION_SHAPES)
  {
    physics->SetPassable(true);
  }
  
  // Finally, add shape to our physicsobject
  physics->AddShape(shape);
  
  return physics;
}

/**
 * @brief Create material at index.
 * @param aIndex Index to create material for.
 * @param aChemistryManager Chemistry manager.
 * @return New material.
 */
ChemistryMaterial* TileMapGenerator::CreateMaterialAtIndex(unsigned const aIndex, ChemistryManager *aChemistryManager)
{
  HashString name = mMaterialNames[aIndex];
  ChemistryMaterial *material = aChemistryManager->CreateMaterial(name);
  return material;
}