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
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cctype>
#include <map>
#include <set>
#include <list>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include "HashString.h"
#include "SmartPointer.h"

#if defined(_WIN32) || defined(__APPLE__) || defined(__linux__)
#define PC 1
#endif

#define DebugLogPrint std::printf
#define TimePointToMilliseconds std::chrono::duration_cast<std::chrono::milliseconds>

namespace Common
{
  std::string const         RelativePath(std::string const &aSubFolder, std::string const &aFileName);

  #ifndef _WIN32
  long                      GetTickCount();
  long                      timeGetTime();
  #endif

  int                       StringToInt(std::string const &aValue);
  float                     StringToFloat(std::string const &aValue);
  bool                      StringToBool(std::string const &aValue);
  std::vector<int>          StringToIntVector(std::string const &aValue);
  std::vector<std::string>  StringToStringVector(std::string const &aValue);

  std::string               IntToString(int const aValue);
  std::string               FloatToString(float const aValue);
  std::string               BoolToString(bool const aValue);
  std::string               IntVectorToString(std::vector<int> &aValue);
  std::string               StringVectorToString(std::vector<std::string> &aValue);

  float                     RandomFloatInRange(float const aX, float const bX);

  typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;
  typedef std::chrono::steady_clock Clock;
  TimePoint                 GetNow();
}

#endif
