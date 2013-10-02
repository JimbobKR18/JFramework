//
//  Sound.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 10/2/13.
//
//

#include "Sound.h"
#include "Common.h"

Sound::Sound()
{
  assert(!"Sound initialized without a name!");
}
Sound::Sound(std::string const &aFilename) : mName(aFilename)
{
  // TODO
}
Sound::~Sound()
{
  delete mData;
}

void Sound::Play()
{
  
}