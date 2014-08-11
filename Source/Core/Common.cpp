//
//  Common.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#include "Common.h"

// Constructors
HashString::HashString() : mString(""), mHash(0)
{
}

HashString::HashString(char const* aString, unsigned aStart, unsigned aEnd)
{
  if(aEnd > strlen(aString))
    assert(!"HashString constructor will run off the end!");
  else if(aEnd == 0)
    mString = aString;
  else
  {
    for(unsigned i = aStart; i <= aEnd; ++i)
    {
      mString.push_back(aString[i]);
    }
  }
  Hash();
}

HashString::HashString(std::string const &aString, unsigned aStart, unsigned aEnd)
{
  // Use previous definition for ease
  *this = HashString(aString.c_str(), aStart, aEnd);
}

// Const Operations
int HashString::Size() const
{
  return mString.length();
}

int HashString::Length() const
{
  return Size();
}

int HashString::Find(HashString const &aString) const
{
  if(aString.Length() == 0)
    assert(!"HashString is empty being passed into Find");

  return mString.find(aString.mString);
}

HashString HashString::SubString(int aStart, int aLength) const
{
  return mString.substr(aStart, aLength);
}

std::vector<HashString> HashString::Split(HashString const &aDelimiter) const
{
  std::vector<HashString> ret;
  unsigned curPos = 0;
  unsigned nextMatch = mString.find(aDelimiter.mString);

  // Catch all in case no delimiter
  if(nextMatch == (unsigned)std::string::npos)
  {
    ret.push_back(mString);
  }
  // Find delimiter and split
  while(nextMatch != (unsigned)std::string::npos)
  {
    ret.push_back(HashString(mString, curPos, nextMatch));
    curPos = nextMatch + 1;
    nextMatch = mString.find(aDelimiter.mString, curPos);
  }
  return ret;
}

// Non-Const Operations
void HashString::Reverse()
{
  std::reverse(mString.begin(), mString.end());
  Hash();
}

void HashString::Push(char aChar)
{
  mString.push_back(aChar);
  Hash();
}

// Operators
void HashString::operator=(HashString const &aRhs)
{
  mString = aRhs.mString;
  mHash = aRhs.mHash;
}

bool HashString::operator==(HashString const &aRhs)
{
  return mHash == aRhs.mHash;
}

char HashString::operator[](int aValue)
{
  return mString[aValue];
}

HashString::operator std::string()
{
  return mString;
}

HashString::operator char const*()
{
  return mString.c_str();
}

HashString HashString::operator+(HashString const &aRhs) const
{
  return HashString(mString + aRhs.mString);
}

void HashString::operator+=(HashString const &aRhs)
{
  mString += aRhs.mString;
  Hash();
}

void HashString::Hash()
{
  mHash = 0;
  char const *key = "srkfadcultjylenoqickbwnxmpvg";
  int len = strlen(key);
  for(unsigned i = 0; i < mString.length(); ++i)
  {
    if(mHash % 2 == 0)
      mHash += (static_cast<int>(mString[i]) + i) ^ key[i % len];
    else
      mHash += (static_cast<int>(mString[i]) + i) & key[i % len];
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

  bool StringToBool(std::string const &aValue)
  {
    bool ret = (aValue == "True") ? true : false;
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

  std::vector<std::string> StringToStringVector(std::string const &aValue)
  {
    std::vector<std::string> ret;
    std::string value;
    for(unsigned i = 0; i < aValue.length(); ++i)
    {
      if(aValue[i] == ',')
      {
        ret.push_back(value);
        value.clear();
      }
      else
      {
        value.push_back(aValue[i]);
      }
    }
    ret.push_back(value);
    return ret;
  }

  std::string IntToString(int const aValue)
  {
    char buffer[33];
    sprintf(buffer, "%d", aValue);
    return std::string(buffer);
  }

  std::string FloatToString(float const aValue)
  {
    std::ostringstream stream;
    stream << aValue;
    return stream.str();
  }

  std::string BoolToString(bool const aValue)
  {
    if(aValue)
      return "true";
    else
      return "false";
  }

  std::string IntVectorToString(std::vector<int> &aValue)
  {
    std::string ret;
    std::vector<int>::iterator end = aValue.end();
    for(std::vector<int>::iterator it = aValue.begin(); it != end; ++it)
    {
      std::ostringstream stream;
      stream << *it;
      ret += stream.str();
      ret += ",";
    }
    ret = ret.substr(0, ret.size() - 2);

    return ret;
  }

  std::string StringVectorToString(std::vector<std::string> &aValue)
  {
    std::string ret;
    std::vector<std::string>::iterator end = aValue.end();
    for(std::vector<std::string>::iterator it = aValue.begin(); it != end; ++it)
    {
      ret += *it;
      ret += ", ";
    }
    ret = ret.substr(0, ret.size() - 2);
    return ret;
  }

  float RandomFloatInRange(float const aX, float const bX)
  {
    float diff = bX - aX;
    diff *= (rand() % 1000) / 1000.0f;
    return aX + diff;
  }

  TimePoint GetNow()
  {
    return Clock::now();
  }
}
