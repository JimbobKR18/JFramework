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
#include "GraphicsManager.h"
#include "ChemistryManager.h"
#include "MathExt.h"
#include "Renderable.h"
#include "Transform.h"

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
    BOTTOMRIGHT,
    BACKSLASH,
    FORWARDSLASH,
    LEFTSIDE,
    RIGHTSIDE,
    TOPSIDE,
    BOTTOMSIDE,
    HALFTOP,
    HALFBOTTOM,
    HALFLEFT,
    HALFRIGHT,
    QUARTERTOPLEFT,
    QUARTERTOPRIGHT,
    QUARTERBOTTOMLEFT,
    QUARTERBOTTOMRIGHT,
    CUBE_PASSABLE,
    TOPLEFT_PASSABLE,
    BOTTOMLEFT_PASSABLE,
    TOPRIGHT_PASSABLE,
    BOTTOMRIGHT_PASSABLE,
    BACKSLASH_PASSABLE,
    FORWARDSLASH_PASSABLE,
    LEFTSIDE_PASSABLE,
    RIGHTSIDE_PASSABLE,
    TOPSIDE_PASSABLE,
    BOTTOMSIDE_PASSABLE,
    HALFTOP_PASSABLE,
    HALFBOTTOM_PASSABLE,
    HALFLEFT_PASSABLE,
    HALFRIGHT_PASSABLE,
    QUARTERTOPLEFT_PASSABLE,
    QUARTERTOPRIGHT_PASSABLE,
    QUARTERBOTTOMLEFT_PASSABLE,
    QUARTERBOTTOMRIGHT_PASSABLE,
    ALL_COLLISION_SHAPES
  };

  // Basics
  int                               mWidth;
  int                               mHeight;
  int                               mTileSize;
  int                               mLayer;
  float                             mZOffset;
  HashString                        mImageName;
  HashString                        mDataName;
  Vector3                           mCollisionOffset;
  std::vector<int>                  mTiles;
  std::vector<int>                  mCollisionData;
  std::vector<int>                  mCollisionShapes;
  std::unordered_map<int, float>    mTileHeights;
  std::unordered_map<int, GameObject*> mObjects;

  // Animated tiles
  float                             mAnimationSpeed;
  float                             mCurrentAnimationTime;
  std::unordered_map<Renderable*, int>           mAnimatedObjects;
  std::unordered_map<int, std::vector<int>>   mAnimations;
  std::unordered_map<int, int>                mCurrentFrames;
  
  // Materials
  std::vector<int>                  mMaterials;
  std::unordered_map<int, HashString> mMaterialNames;
  
  // Etc.
  std::unordered_map<int, bool>     mEmptyTiles;
  Renderable::IsolatedRenderLayerContainer mIsolatedRenderingLayers;

  // Level owning this generator
  Level*                          mOwner;
  
public:
  TileMapGenerator();
  TileMapGenerator(int aWidth, int aHeight, int aTileSize, int aLayer, float aZOffset,
                   HashString const &aImageName,
                   HashString const &aDataName,
                   Vector3 const &aCollisionOffset,
                   std::vector<int> const &aTiles,
                   std::vector<int> const &aCollisionData, 
                   std::vector<int> const &aCollisionShapes,
                   std::vector<int> const &aMaterialData,
                   std::unordered_map<int, float> const &aTileHeights, 
                   std::unordered_map<int, HashString> const &aMaterials,
                   std::unordered_map<int, bool> const &aEmptyTiles,
                   Renderable::IsolatedRenderLayerContainer const &aIsolatedRenderingLayers,
                   std::unordered_map<int, std::vector<int>> const &aAnimations,
                   float const aAnimationSpeed, Level *aOwner);
  ~TileMapGenerator();

  void              Update();

  int const&        GetWidth() const;
  int const&        GetHeight() const;
  int const&        GetTileSize() const;
  HashString const& GetImageName() const;
  HashString const& GetDataName() const;
  std::vector<int>& GetArtTiles();
  std::vector<int>& GetCollisionTiles();
  std::vector<int>& GetCollisionShapes();
  std::unordered_map<int, float>& GetTileHeights();

  GameObject*       GetObject(int const aX, int const aY);
  GameObject*       GetObject(int const aIndex);
  std::unordered_map<int, GameObject*> GetObjects();
  int const&        GetTileValue(int const aX, int const aY);
  int const&        GetCollisionValue(int const aX, int const aY);
  int const&        GetMaterialValue(int const aX, int const aY);
  int               GetIndex(int const aX, int const aY);
  std::vector<int>  GetIndices(int const aX, int const aY, int const bX, int const bY);

  void              Serialize(ParserNode *aNode);
  
private:
  void              CreateTilesInRange(unsigned const aXStart, unsigned const aYStart, 
                      unsigned const aXEnd, unsigned const aYEnd, Vector3 const &aTileSize, 
                      ObjectManager *aObjectManager, GraphicsManager *aGraphicsManager, 
                      PhysicsWorld *aPhysicsWorld, ChemistryManager *aChemistryManager);
  PhysicsObject*    CreatePhysicsAtIndex(unsigned const aIndex, PhysicsWorld *aPhysicsWorld, 
                      Transform *aTransform, Vector3 const &aZeroVector, Vector3 const &aCollisionOffset, 
                      unsigned const aCollisionDataVectorSize, float aZPos);
  ChemistryMaterial* CreateMaterialAtIndex(unsigned const aIndex, ChemistryManager *aChemistryManager);
};

#endif /* defined(__JFramework__TileMapGenerator__) */
