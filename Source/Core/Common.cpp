//
//  Common.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#include "Common.h"

std::string const RelativePath(std::string const &aFileName)
{
  std::string ret = "LUAFiles/";
  ret.append(aFileName);
  return ret;
}
