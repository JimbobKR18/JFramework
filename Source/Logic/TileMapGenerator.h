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

class Level;

class TileMapGenerator
{
private:
  int                       mWidth;
  int                       mHeight;
  int                       mTileSize;
  std::string               mImageName;
  std::string               mDataName;
  std::vector<int>          mTiles;
  std::vector<int>          mCollision;
  std::vector<GameObject*>  mObjects;
  Level*                    mOwner;
  
public:
  TileMapGenerator();
  TileMapGenerator(int aWidth, int aHeight, int aTileSize,
                   std::string const &aImageName,
                   std::string const &aDataName,
                   std::vector<int> const &aTiles,
                   std::vector<int> const &aCollision, Level *aOwner);
  ~TileMapGenerator();

  int               GetWidth() const;
  int               GetHeight() const;
  int               GetTileSize() const;
  std::string       GetImageName() const;
  std::string       GetDataName() const;
  std::vector<int>& GetArtTiles();
  std::vector<int>& GetCollisionTiles();

  GameObject*       GetObject(int const aX, int const aY);
  int               GetTileValue(int const aX, int const aY);
  int               GetCollisionValue(int const aX, int const aY);
  int               GetIndex(int const aX, int const aY);

  void              Serialize(Parser &aParser);
};

#endif /* defined(__JFramework__TileMapGenerator__) */
