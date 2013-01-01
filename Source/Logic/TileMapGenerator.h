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
#include "Level.h"

class TileMapGenerator
{
private:
  int mWidth, mHeight, mTileSize;
  std::string mFilename;
  std::vector<int> mTiles;
  Level *mOwner;
  
  typedef std::vector<int>::const_iterator TilesIT;
public:
  TileMapGenerator();
  TileMapGenerator(int aWidth, int aHeight, int aTileSize,
                   std::string const &aFilename,
                   std::vector<int> const &aTiles, Level *aOwner);
  ~TileMapGenerator();
};

#endif /* defined(__JFramework__TileMapGenerator__) */
