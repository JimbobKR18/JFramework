/*
 * Constants.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: jimbob
 */

#include "Constants.h"

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
  TextParser parser(Common::RelativePath("Game", "Constants.txt"), false);
  while(parser.IsGood())
  {
    std::string key, value;
    parser.GetNextString(key);
    parser.GetNextString(value);
    mValues[key] = value;
  }
}
