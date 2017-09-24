//
//  Sound.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 10/2/13.
//
//

#include "Sound.h"

Sound::Sound()
{
  assert(!"Sound initialized without a name!");
}

Sound::Sound(HashString const &aFilename)
{
  mName = Common::RetrieveNameFromFileName(aFilename);

  std::ifstream infile(aFilename.ToString(), std::ifstream::binary);
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

HashString Sound::GetName() const
{
  return mName;
}

void Sound::SetName(HashString const &aName)
{
  mName = aName;
}
