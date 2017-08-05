/*
 * Constants.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: jimbob
 */

#include "Constants.h"
#include "LUATypes.h"

Constants::ValueContainer Constants::mValues;

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
HashString const &Constants::GetString(HashString const &aValue)
{
  ValueContainerIT it = mValues.find(aValue.ToHash());

  if(it == mValues.end())
    assert(!"Value not found!");

  return it->second;
}

/**
 * @brief Get int from values map, asserts if nothign found.
 * @param aValue Value to find.
 */
int const Constants::GetInteger(HashString const &aValue)
{
  return GetString(aValue).ToInt();
}

/**
 * @brief Get float from values map, asserts if nothign found.
 * @param aValue Value to find.
 */
float const Constants::GetFloat(HashString const &aValue)
{
  return GetString(aValue).ToFloat();
}

/**
 * @brief Get bool from values map, asserts if nothign found.
 * @param aValue Value to find.
 */
bool const Constants::GetBoolean(HashString const &aValue)
{
  return GetString(aValue).ToBool();
}

/**
 * @brief Read in values from file
 */
void Constants::Deserialize()
{
  // Read line by line, much easier
  mValues.clear();
  std::ifstream infile(Common::RelativePath("Game", "Constants.txt"));
  while(infile.good() && !infile.eof())
  {
    std::string key, value, empty;
    infile >> key;
    infile >> empty;
    infile >> value;
    mValues[Common::StringHashFunction(key)] = Common::ParseString(&infile, value);
  }
  infile.close();
}

void Constants::SerializeLUA()
{
  SLB::Class<Constants>("Constants").set("GetString", &Constants::GetString)
                                    .set("GetInteger", &Constants::GetInteger)
                                    .set("GetFloat", &Constants::GetFloat)
                                    .set("GetBoolean", &Constants::GetBoolean);
}
