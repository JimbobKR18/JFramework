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
  typedef std::map<HashString, HashString>::iterator valIT;
  static std::map<HashString, HashString> mValues;

public:
  Constants();
  virtual ~Constants();

  static HashString GetString(HashString const &aValue);
  static int GetInteger(HashString const &aValue);
  static float GetFloat(HashString const &aValue);
  static bool GetBoolean(HashString const &aValue);
  static void Deserialize();
};

#endif /* CONSTANTS_H_ */
