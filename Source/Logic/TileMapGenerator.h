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

class Level;

class TileMapGenerator
{
private:
  int               mWidth,
                    mHeight,
                    mTileSize;
  std::string       mFilename;
  std::vector<int>  mTiles,
                    mCollision;
  Level*            mOwner;
  
public:
  TileMapGenerator();
  TileMapGenerator(int aWidth, int aHeight, int aTileSize,
                   std::string const &aFilename,
                   std::vector<int> const &aTiles,
                   std::vector<int> const &aCollision, Level *aOwner);
  ~TileMapGenerator();

  int               GetWidth() const;
  int               GetHeight() const;
  int               GetTileSize() const;
  std::string       GetFilename() const;
  std::vector<int>& GetArtTiles();
  std::vector<int>& GetCollisionTiles();

  int               GetTileValue(int aX, int aY);
  int               GetCollisionValue(int aX, int aY);
};

#endif /* defined(__JFramework__TileMapGenerator__) */
