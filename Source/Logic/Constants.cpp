/*
 * Constants.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: jimbob
 */

#include "Constants.h"
#include <fstream>

std::map<HashString, HashString> Constants::mValues;

Constants::Constants()
{
}

Constants::~Constants()
{
}

/**
 * @brief Get string from values map, asserts if nothign found.
 * @param aValue Value to find.
 */
HashString Constants::GetString(HashString const &aValue)
{
  valIT it = mValues.find(aValue);

  if(it == mValues.end())
    assert(!"Value not found!");

  return it->second;
}

/**
 * @brief Get int from values map, asserts if nothign found.
 * @param aValue Value to find.
 */
int Constants::GetInteger(HashString const &aValue)
{
  return GetString(aValue).ToInt();
}

/**
 * @brief Get float from values map, asserts if nothign found.
 * @param aValue Value to find.
 */
float Constants::GetFloat(HashString const &aValue)
{
  return GetString(aValue).ToFloat();
}

/**
 * @brief Get bool from values map, asserts if nothign found.
 * @param aValue Value to find.
 */
bool Constants::GetBoolean(HashString const &aValue)
{
  return GetString(aValue).ToBool();
}

/**
 * @brief Read in values from file
 */
void Constants::Deserialize()
{
  // Read line by line, much easier
  std::ifstream infile(Common::RelativePath("Game", "Constants.txt"));
  while(infile.good() && !infile.eof())
  {
    std::string key, value, empty;
    infile >> key;
    infile >> empty;
    infile >> value;
    mValues[key] = value;
  }
  infile.close();
}
