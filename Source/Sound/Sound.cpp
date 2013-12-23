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
Sound::Sound(std::string const &aFilename)
{
  // Get file name
  for(int i = static_cast<int>(aFilename.size()) - 1;
      aFilename[i] != '/' && i >= 0; --i)
  {
    mName.push_back(aFilename[i]);
  }
  std::reverse(mName.begin(), mName.end());

  std::ifstream infile(aFilename, std::ifstream::binary);
  infile.seekg(0, infile.end);
  mLength = infile.tellg();
  infile.seekg(0);

  mData = new unsigned char[mLength];
  infile.read((char*)mData, mLength);
  infile.close();
}
Sound::~Sound()
{
  delete mData;
}
