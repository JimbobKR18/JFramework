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

#if defined(_WIN32) || defined(__APPLE__) || defined(__linux__)
#define PC 1
#endif

#define DebugLogPrint std::printf
#define TimePointToMilliseconds std::chrono::duration_cast<std::chrono::milliseconds>

// Basically an extension of std::string, now with hashing.
class HashString
{
private:
  std::string mString;
  int         mHash;

  std::string::iterator stringIT;
  std::string::const_iterator constStringIT;
public:
  // Constructors
  HashString();
  HashString(char const* aString, unsigned aStart = 0, unsigned aEnd = 0);
  HashString(std::string const &aString, unsigned aStart = 0, unsigned aEnd = 0);

  // Const Operations
  int Size() const;
  int Length() const;
  int Find(HashString const &aString) const;
  HashString SubString(int aStart, int aLength) const;
  std::vector<HashString> Split(HashString const &aDelimiter) const;

  // Non-Const Operations
  void Reverse();
  void Push(char aChar);

  // Operators
  // Equality
  void operator=(HashString const &aRhs);
  // Comparison
  bool operator==(HashString const &aRhs);
  char operator[](int aValue);
  // Conversion
  operator std::string();
  operator char const*();
private:
  void Hash();
};

template<typename T>
class SmartPointer
{
private:
  T*  mPointer;
public:
  SmartPointer() : mPointer(NULL) {}
  SmartPointer(T *aPointer) : mPointer(aPointer) {}
  SmartPointer(SmartPointer<T> &aPointer) { mPointer = new T(*aPointer); }
  ~SmartPointer() { if(mPointer) delete mPointer; }
  T& operator*() { return *mPointer; }
  T* operator->() { return mPointer; }
  operator T*() { return mPointer; }
  operator bool() { return mPointer != NULL; }
  bool isNULL() const { return mPointer == NULL; }
  void operator=(T *aPtr) { ReassignValue(aPtr); }
  void operator=(SmartPointer<T> &aPtr) { ReassignValue(aPtr.mPointer); }
  bool operator==(T *aPtr) { return isEqual(aPtr); }
  bool operator==(SmartPointer<T> &aPtr) { return isEqual(aPtr.mPointer); }
private:
  void ReassignValue(T *aPtr)
  {
    //if(aPtr == NULL)
      //delete mPointer;
    mPointer = aPtr;
  }
  bool isEqual(T *aPtr)
  {
    return aPtr == mPointer;
  }
};

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
