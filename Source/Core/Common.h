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
#include <iterator>
#include <cfloat>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <chrono>
#include <ctime>
#include <exception>
#include "HashString.h"
#include "SmartPointer.h"
#include "ObjectPlacement.h"

#if defined(_WIN32) || defined(__APPLE__) || defined(__linux__)
  #define PC 1
#endif

#define DebugLogPrint std::printf

namespace Common
{
  std::string const         RelativePath(std::string const &aSubFolder, std::string const &aFileName);

  int                       StringToInt(std::string const &aValue);
  float                     StringToFloat(std::string const &aValue);
  double                    StringToDouble(std::string const &aValue);
  bool                      StringToBool(std::string const &aValue);
  std::vector<int>          StringToIntVector(std::string const &aValue);
  std::vector<float>        StringToFloatVector(std::string const &aValue);
  std::vector<double>       StringToDoubleVector(std::string const &aValue);
  std::vector<bool>         StringToBoolVector(std::string const &aValue);
  std::vector<std::string>  StringToStringVector(std::string const &aValue);
  std::vector<HashString>   StringToHashStringVector(std::string const &aValue);
  
  std::set<int>             StringToIntSet(std::string const &aValue);
  std::set<std::string>     StringToStringSet(std::string const &aValue);
  std::set<HashString>      StringToHashStringSet(std::string const &aValue);

  std::string               IntToString(int const aValue);
  std::string               FloatToString(float const aValue);
  std::string               DoubleToString(double const aValue);
  std::string               BoolToString(bool const aValue);
  std::string               IntVectorToString(std::vector<int> const &aValue);
  std::string               FloatVectorToString(std::vector<float> const &aValue);
  std::string               DoubleVectorToString(std::vector<double> const &aValue);
  std::string               BoolVectorToString(std::vector<bool> const &aValue);
  std::string               StringVectorToString(std::vector<std::string> const &aValue);
  std::string               HashStringVectorToString(std::vector<HashString> const &aValue);
  
  std::string               IntSetToString(std::set<int> const &aValue);
  std::string               StringSetToString(std::set<std::string> const &aValue);
  std::string               HashStringSetToString(std::set<HashString> const &aValue);
  
  std::string               ParseString(std::istream *infile, std::string const &aString);
  std::string               TrimString(std::string const &aString);
  
  HashString                RetrieveNameFromFileName(HashString const &aFileName);
  int                       StringHashFunction(std::string const &aString);

  int                       RandomIntInRange(int const aX, int const bX);
  float                     RandomFloatInRange(float const aX, float const bX);
  
  int                       GetCurrentYear();
  int                       GetCurrentMonth();
  int                       GetCurrentDayOfMonth();
  
  template<typename T>      
  std::unordered_map<int, T> ConvertVectorToHashMap(std::vector<T> const &aVector);
}

#endif
