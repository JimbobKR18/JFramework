//
//  Common.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/1/12.
//
//

#ifndef JFramework_Common_h
#define JFramework_Common_h

#include <algorithm>
#include <vector>
#include <string>
#include <cassert>
#include <cstdlib>
#include <cctype>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <iostream>

#if defined(_WIN32) || defined(__APPLE__) || defined(__linux__)
#define PC 1
#endif

namespace Common
{
  std::string const RelativePath(std::string const &aFileName);

  #ifndef _WIN32
  long GetTickCount();
  long timeGetTime();
  #endif

  int StringToInt(std::string const &aValue);
  float StringToFloat(std::string const &aValue);
  std::vector<int> StringToIntVector(std::string const &aValue);
}

#endif
