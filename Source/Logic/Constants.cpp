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
 * @brief Get string from values map, asserts if nothing found.
 * @param aValue Value to find.
 */
HashString const &Constants::GetString(HashString const &aValue)
{
  ValueContainerIT it = mValues.find(aValue.ToHash());

  if(it == mValues.end())
  {
    DebugLogPrint("Value %s not found!\n", aValue.ToCharArray());
    assert(!"Value not found!");
  }

  return it->second;
}

/**
 * @brief Get int from values map, asserts if nothing found.
 * @param aValue Value to find.
 */
int const Constants::GetInteger(HashString const &aValue)
{
  return GetString(aValue).ToInt();
}

/**
 * @brief Get float from values map, asserts if nothing found.
 * @param aValue Value to find.
 */
float const Constants::GetFloat(HashString const &aValue)
{
  return GetString(aValue).ToFloat();
}

/**
 * @brief Get bool from values map, asserts if nothing found.
 * @param aValue Value to find.
 */
bool const Constants::GetBoolean(HashString const &aValue)
{
  return GetString(aValue).ToBool();
}

/**
 * @brief Get int vector from values map, asserts if nothing found.
 * @param aValue Value to find.
 */
std::vector<int> const Constants::GetIntegerVector(HashString const &aValue)
{
  return GetString(aValue).ToIntVector();
}

/**
 * @brief Get float vector from values map, asserts if nothing found.
 * @param aValue Value to find.
 */
std::vector<float> const Constants::GetFloatVector(HashString const &aValue)
{
  return GetString(aValue).ToFloatVector();
}

/**
 * @brief Get bool vector from values map, asserts if nothing found.
 * @param aValue Value to find.
 */
std::vector<bool> const Constants::GetBooleanVector(HashString const &aValue)
{
  return GetString(aValue).ToBoolVector();
}

/**
 * @brief Get Vector3 from values map, asserts if nothing found.
 * @param aValue Value to find.
 */
Vector3 const Constants::GetVector3(HashString const &aValue)
{
  return GetString(aValue).ToVector3();
}

/**
 * @brief Get Vector4 from values map, asserts if nothing found.
 * @param aValue Value to find.
 */
Vector4 const Constants::GetVector4(HashString const &aValue)
{
  return GetString(aValue).ToVector4();
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
  // NOTE: LUA does not understand const floats, ints, bool, etc. Call GetString and call conversion function instead.
  SLB::Class<Constants, SLB::Instance::NoCopyNoDestroy>("Constants")
    .set("GetString", &Constants::GetString);
}
