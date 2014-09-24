/*
 * Constants.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: jimbob
 */

#include "Constants.h"
#include <fstream>

std::map<std::string, std::string> Constants::mValues;

Constants::Constants()
{
}

Constants::~Constants()
{
}

std::string Constants::GetString(std::string const &aValue)
{
  valIT it = mValues.find(aValue);

  if(it == mValues.end())
    assert(!"Value not found!");

  return it->second;
}

int Constants::GetInteger(std::string const &aValue)
{
  return Common::StringToInt(GetString(aValue));
}

float Constants::GetFloat(std::string const &aValue)
{
  return Common::StringToFloat(GetString(aValue));
}

bool Constants::GetBoolean(std::string const &aValue)
{
  return Common::StringToBool(GetString(aValue));
}

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
