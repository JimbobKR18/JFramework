/*
 * Constants.h
 *
 *  Created on: Apr 10, 2014
 *      Author: jimbob
 */

#ifndef __JFramework_Constants_h_
#define __JFramework_Constants_h_

#include "Common.h"
#include "TextParser.h"

class Constants
{
private:
  typedef std::map<HashString, HashString> ValueContainer;
  typedef ValueContainer::iterator ValueContainerIT;
  static ValueContainer mValues;

public:
  Constants();
  virtual ~Constants();

  static HashString const &GetString(HashString const &aValue);
  static int const GetInteger(HashString const &aValue);
  static float const GetFloat(HashString const &aValue);
  static bool const GetBoolean(HashString const &aValue);
  static void Deserialize();
  
private:
  static std::string ParseLiteral(std::ifstream *infile, std::string const &aLiteral);
};

#endif /* CONSTANTS_H_ */
