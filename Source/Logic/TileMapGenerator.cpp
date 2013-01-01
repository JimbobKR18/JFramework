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

TileMapGenerator::TileMapGenerator()
{
  assert(0);
}

TileMapGenerator::TileMapGenerator(int aWidth, int aHeight, int aTileSize,
                                   std::string const &aFilename,
                                   std::vector<int> const &aTiles,
                                   Level *aOwner) :
                                   mWidth(aWidth), mHeight(aHeight),
                                   mTileSize(aTileSize), mFilename(aFilename),
                                   mTiles(aTiles), mOwner(aOwner)
{
  int xPos = 0, yPos = 0;
  float halfX = mWidth * aTileSize;
  float halfY = mHeight * aTileSize;
  for(TilesIT it = mTiles.begin(); it != mTiles.end(); ++it)
  {
    // Make GameObject to place
    GameObject *obj = new GameObject(mFilename);
    mOwner->GetManager()->GetOwningApp()->GET<ObjectManager>()->ParseObject(obj);
    
    // Get Transform of new object
    Transform *transform = obj->GET<Transform>();
    transform->SetPosition(Vector3(-halfX + (aTileSize * 2 * xPos),
                                   -halfY + (aTileSize * 2 * yPos),0));
    transform->SetSize(Vector3(mTileSize, mTileSize, 0));
    
    Surface *surface = obj->GET<Surface>();
    surface->SetAnimated(false);
    surface->GetTextureData()->SetCurrentFrame(*it);
    
    mOwner->AddObject(obj);
    
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