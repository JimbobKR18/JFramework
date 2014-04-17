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
  typedef std::map<std::string, std::string>::iterator valIT;
  static std::map<std::string, std::string> mValues;

public:
  Constants();
  virtual ~Constants();

  static std::string GetString(std::string const &aValue);
  static int GetInteger(std::string const &aValue);
  static float GetFloat(std::string const &aValue);
  static bool GetBoolean(std::string const &aValue);
  static void Deserialize();
};

#endif /* CONSTANTS_H_ */
