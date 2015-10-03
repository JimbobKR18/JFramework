//
//  TileMapGenerator.h
//  JFramework
//
//  Created by Jimmy Spencer on 12/31/12.
//  The idea: A helper class to generate tilemaps
//

#ifndef __JFramework__TileMapGenerator__
#define __JFramework__TileMapGenerator__

#include "Common.h"
#include "Parser.h"
#include "GameObject.h"
#include "PhysicsWorld.h"
#include "ObjectManager.h"
#include "MathExt.h"
#include "Surface.h"

class Level;

class TileMapGenerator
{
private:
  enum CollisionShapes
  {
    EMPTY = 0,
    CUBE,
    TOPLEFT,
    BOTTOMLEFT,
    TOPRIGHT,
    BOTTOMRIGHT
  };

  // Basics
  int                             mWidth;
  int                             mHeight;
  int                             mTileSize;
  std::string                     mImageName;
  std::string                     mDataName;
  std::vector<int>                mTiles;
  std::vector<int>                mCollisionData;
  std::vector<int>                mCollisionShapes;
  std::map<int, float>            mTileHeights;
  std::vector<GameObject*>        mObjects;

  // Animated tiles
  float                           mAnimationSpeed;
  float                           mCurrentAnimationTime;
  std::map<Surface*, int>         mAnimatedObjects;
  std::map<int, std::vector<int>> mAnimations;
  std::map<int, int>              mCurrentFrames;

  // Level owning this generator
  Level*                          mOwner;
  
public:
  TileMapGenerator();
  TileMapGenerator(int aWidth, int aHeight, int aTileSize,
                   std::string const &aImageName,
                   std::string const &aDataName,
                   std::vector<int> const &aTiles,
                   std::vector<int> const &aCollisionData, 
                   std::vector<int> const &aCollisionShapes,
                   std::map<int, float> const &aTileHeights, 
                   std::map<int, std::vector<int>> const &aAnimations,
                   float const aAnimationSpeed, Level *aOwner);
  ~TileMapGenerator();

  void              Update();

  int               GetWidth() const;
  int               GetHeight() const;
  int               GetTileSize() const;
  std::string       GetImageName() const;
  std::string       GetDataName() const;
  std::vector<int>& GetArtTiles();
  std::vector<int>& GetCollisionTiles();
  std::vector<int>& GetCollisionShapes();
  std::map<int, float>& GetTileHeights();

  GameObject*       GetObject(int const aX, int const aY);
  GameObject*       GetObject(int const aIndex);
  int               GetTileValue(int const aX, int const aY);
  int               GetCollisionValue(int const aX, int const aY);
  int               GetIndex(int const aX, int const aY);
  std::vector<int>  GetIndices(int const aX, int const aY, int const bX, int const bY);

  void              Serialize(Parser &aParser);
  
private:
  void              CreateTilesInRange(unsigned const aStart, unsigned const aEnd, Vector3 const &aTileSize, ObjectManager *aObjectManager, PhysicsWorld *aPhysicsWorld);
};

#endif /* defined(__JFramework__TileMapGenerator__) */
