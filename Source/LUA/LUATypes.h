/*
 * LUATypes.h
 *
 *  Created on: Dec 15, 2012
 *      Author: jimmy
 */

#ifndef LUATYPES_H_
#define LUATYPES_H_

#include "Core/LuaIncludes.h"

namespace LUABind
{
  void Initialize();
  void RegisterClasses();
  void LoadScriptFromFile(std::string const &aFilename);
  void LoadScriptFromString(std::string const &aString);
};

#endif /* LUATYPES_H_ */
