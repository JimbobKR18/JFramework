#include "SystemProperties.h"

/* This class is meant to be read in from a file in the same location as
 * the executable. */

HashString SystemProperties::mAssetsDirectory = HashString();

SystemProperties::SystemProperties()
{
}

SystemProperties::~SystemProperties()
{
}

/**
 * @brief Get assets directory
 * @return Assests directory as a HashString.
 */
HashString const &SystemProperties::GetAssetsDirectory()
{
  return mAssetsDirectory;
}

/**
 * @brief Read in from file.
 */
void SystemProperties::Deserialize()
{
  std::ifstream infile("./SystemProperties.ini");
  if(!infile.good())
    assert(!"No system property file found, please place one in same directory as executable.");
    
  while(infile.good() && !infile.eof())
  {
    std::string key, value, empty;
    infile >> key;
    infile >> empty;
    infile >> value;
    
    if(key == "AssetsDirectory")
      mAssetsDirectory = value;
    else if(!key.empty())
      assert(!"Invalid property found.");
  }
  infile.close();
}