//
//  Common.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#include "Common.h"

JString::JString() : mString(""), mHash(0)
{
}

JString::JString(char const* aString) : mString(aString)
{
  Hash();
}

JString::JString(std::string const &aString) : mString(aString)
{
  Hash();
}

void JString::operator=(JString const &aRhs)
{
  mString = aRhs.mString;
  mHash = aRhs.mHash;
}

bool JString::operator==(JString const &aRhs)
{
  return mHash == aRhs.mHash;
}

char JString::operator[](int aValue)
{
  return mString[aValue];
}

void JString::Hash()
{
  mHash = 0;
  char const *key = "srkfadcult";
  int len = strlen(key);
  for(unsigned i = 0; i < mString.length(); ++i)
  {
    mHash += (static_cast<int>(mString[i]) + i) ^ key[i % len];
  }
}

namespace Common
{
  #ifdef __APPLE__
  #include <mach/mach_time.h>
  #define ORWL_NANO (+1.0E-9)
  #define ORWL_GIGA UINT64_C(1000000000)
  #define CLOCK_MONOTONIC 0

  double orwl_timebase = 0.0;
  uint64_t orwl_timestart = 0;

  // A mac replacement for the linux function clock_gettime
  void clock_gettime(int aEmpty, struct timespec *aTime)
  {
    if (!orwl_timestart)
    {
      mach_timebase_info_data_t tb = { 0 };
      mach_timebase_info(&tb);
      orwl_timebase = tb.numer;
      orwl_timebase /= tb.denom;
      orwl_timestart = mach_absolute_time();
    }
    
    double diff = (mach_absolute_time() - orwl_timestart) * orwl_timebase;
    aTime->tv_sec = diff * ORWL_NANO;
    aTime->tv_nsec = diff - (aTime->tv_sec * ORWL_GIGA);
  }
  #endif

  std::string const RelativePath(std::string const &aSubFolder, std::string const &aFileName)
  {
  #ifdef _WIN32
    std::string ret;
  #elif defined(__APPLE__)
    std::string ret = "/Users/jimmyspencer/Documents/JFramework/Assets/";
  #else
    std::string ret = "../Assets/";
  #endif
    ret.append(aSubFolder);
    ret.append("/");
    ret.append(aFileName);
    return ret;
  }

  #ifndef _WIN32
  long GetTickCount()
  {
    return timeGetTime();
  }

  long timeGetTime()
  {
    // Returns time in milliseconds
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec * 1000) + (now.tv_nsec / (1000 * 1000));
  }
  #endif

  int StringToInt(std::string const &aValue)
  {
    int ret = atoi(aValue.c_str());
    return ret;
  }

  float StringToFloat(std::string const &aValue)
  {
    float ret = atof(aValue.c_str());
    return ret;
  }

  std::vector<int> StringToIntVector(std::string const &aValue)
  {
    std::vector<int> ret;
    std::istringstream stream(aValue);
    
    int value;
    
    while(!stream.eof())
    {
      stream >> value;
      ret.push_back(value);
      stream.ignore();
    }
    
    return ret;
  }

  std::string IntToString(int aValue)
  {
    char buffer[33];
    sprintf(buffer, "%d", aValue);
    return std::string(buffer);
  }
}
