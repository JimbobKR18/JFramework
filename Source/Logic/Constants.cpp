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

std::string Constants::GetValue(std::string const &aValue)
{
  valIT it = mValues.find(aValue);

  if(it == mValues.end())
    assert(!"Value not found!");

  return it->second;
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
