/*
 * Constants.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: jimbob
 */

#include "Constants.h"
#include <fstream>

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
  ValueContainerIT it = mValues.find(aValue);

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
  std::ifstream infile(Common::RelativePath("Game", "Constants.txt"));
  while(infile.good() && !infile.eof())
  {
    std::string key, value, empty;
    infile >> key;
    infile >> empty;
    infile >> value;
    mValues[key] = ParseLiteral(&infile, value);
  }
  infile.close();
}

/**
 * @brief If find "Literal(" keep forming string until find ")"
 * @param aLiteral Literal to parse through.
 */
std::string Constants::ParseLiteral(std::ifstream *infile, std::string const &aLiteral)
{
  std::string ret;
  int literalLocation = aLiteral.find("Literal");
  if(literalLocation == std::string::npos)
    return aLiteral;
  else
  {
    // Getting the full string
    unsigned pos = literalLocation + 8;
    char next;
    bool earlyout = false;
    // Literal(blah) is one whole word, extract
    while(pos < aLiteral.length())
    {
      char next = aLiteral[pos];
      if(next == '\n')
      {
        earlyout = true;
        break;
      }
      if(next != ')' && next != '(')
        ret.push_back(next);
      ++pos;
    }
    // Didn't hit newline, get rest of sentence
    while(!earlyout && infile->get(next))
    {
      if(next == '\n')
        break;
      if(next != ')' && next != '(')
        ret.push_back(next);
    }
  }
  return ret;
}
