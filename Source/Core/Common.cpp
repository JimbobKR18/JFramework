//
//  Common.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#include "Common.h"

// Two macros ensures the macro passed will
// be expanded before being converted to a string.
#define STRINGIZE_DETAIL(str) #str
#define STRINGIZE(str) STRINGIZE_DETAIL(str)

namespace Common
{
  // Mac specific functions
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

  /**
   * @brief Gets relative path for asset files, can be either set by macro ASSET_DIRECTORY
   *        or automatically.
   * @param aSubFolder Sub folder to search inside of asset directory.
   * @param aFileName File to find (extension included)
   */
  std::string const RelativePath(std::string const &aSubFolder, std::string const &aFileName)
  {
    // Manually setting the asset directory
    // Or use defaults based on OS
  #ifdef ASSET_DIRECTORY
    std::string ret = STRINGIZE(ASSET_DIRECTORY);
  #elif defined(_WIN32)
    std::string ret = "../../../Assets/";
  #elif defined(__APPLE__)
    std::string ret = "/Users/jimmyspencer/Documents/project-chang-e/Assets/";
  #else // Linux
    std::string ret = "../Assets/";
  #endif
    ret.append(aSubFolder);
    ret.append("/");
    ret.append(aFileName);
    return ret;
  }

  // Functions that Windows provides that need to be set for Mac and Linux
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

  /**
   * @brief Converts String to Int
   * @param aValue
   */
  int StringToInt(std::string const &aValue)
  {
    int ret = atoi(aValue.c_str());
    return ret;
  }

  /**
   * @brief Converts String to Float
   * @param aValue
   */
  float StringToFloat(std::string const &aValue)
  {
    float ret = atof(aValue.c_str());
    return ret;
  }

  /**
   * @brief Converts String to Boolean, hard set to look for "true"
   * @param aValue
   */
  bool StringToBool(std::string const &aValue)
  {
    bool ret = (aValue == "true") ? true : false;
    return ret;
  }

  /**
   * @brief Converts String to Int Vector
   * @param aValue
   */
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
  
  /**
   * @brief Converts String to Float Vector
   * @param aValue
   */
  std::vector<float> StringToFloatVector(std::string const &aValue)
  {
    std::vector<float> ret;
    std::istringstream stream(aValue);
    
    float value;
    
    while(!stream.eof())
    {
      stream >> value;
      ret.push_back(value);
      stream.ignore();
    }
    
    return ret;
  }

  /**
   * @brief Converts String to String Vector
   * @param aValue
   */
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

  /**
   * @brief Converts Int to String
   * @param aValue
   */
  std::string IntToString(int const aValue)
  {
    char buffer[33];
    sprintf(buffer, "%d", aValue);
    return std::string(buffer);
  }

  /**
   * @brief Converts Float to String
   * @param aValue
   */
  std::string FloatToString(float const aValue)
  {
    std::ostringstream stream;
    stream << aValue;
    return stream.str();
  }

  /**
   * @brief Converts Boolean to String
   * @param aValue
   */
  std::string BoolToString(bool const aValue)
  {
    if(aValue)
      return "true";
    else
      return "false";
  }

  /**
   * @brief Converts Int Vector to String (comma separated)
   * @param aValue
   */
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
    ret = ret.substr(0, ret.size() - 1);

    return ret;
  }
  
  /**
   * @brief Converts Float Vector to String (comma separated)
   * @param aValue
   */
  std::string FloatVectorToString(std::vector<float> &aValue)
  {
    std::string ret;
    std::vector<float>::iterator end = aValue.end();
    for(std::vector<float>::iterator it = aValue.begin(); it != end; ++it)
    {
      std::ostringstream stream;
      stream << *it;
      ret += stream.str();
      ret += ",";
    }
    ret = ret.substr(0, ret.size() - 1);

    return ret;
  }

  /**
   * @brief Converts String Vector to String (comma separated)
   * @param aValue
   */
  std::string StringVectorToString(std::vector<std::string> &aValue)
  {
    std::string ret;
    std::vector<std::string>::iterator end = aValue.end();
    for(std::vector<std::string>::iterator it = aValue.begin(); it != end; ++it)
    {
      ret += *it;
      ret += ",";
    }
    
    // Remove last comma
    ret = ret.substr(0, ret.size() - 1);
    return ret;
  }
  
  /**
   * @brief Get bottom level filename (last sequence after last /)
   * @param aFileName
   */
  HashString RetrieveNameFromFileName(HashString const &aFileName)
  {
    HashString ret;
    
    for(int i = static_cast<int>(aFileName.Size()) - 1;
      aFileName[i] != '/' && i >= 0; --i)
    {
      ret.Push(aFileName[i]);
    }
    ret.Reverse();
    
    return ret;
  }
  
  /**
   * @brief String hash function, good enough default
   * @param aString
   */
  int StringHashFunction(std::string const &aString)
  {
    int hashValue = 0;
    char const *key = "srkfadcultjylenoqickbwnxmpvg";
    int len = strlen(key);
    for(unsigned i = 0; i < aString.length(); ++i)
    {
      if(hashValue % 2 == 0)
        hashValue += (static_cast<int>(aString[i]) + i) ^ key[i % len];
      else
        hashValue += (static_cast<int>(aString[i]) + i) & key[i % len];
    }
    return hashValue;
  }

  /**
   * @brief Get random Int within a range.
   * @param aX Minimum (inclusive)
   * @param bX Maximum (exclusive)
   */
  int RandomIntInRange(int const aX, int const bX)
  {
    return aX + (rand() % (bX - aX));
  }

  /**
   * @brief Get random Float within a range.
   * @param aX Minimum (inclusive)
   * @param bX Maximum (exclusive)
   */
  float RandomFloatInRange(float const aX, float const bX)
  {
    float diff = bX - aX;
    diff *= (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    return aX + diff;
  }

  /**
   * @brief Get clock now
   */
  TimePoint GetNow()
  {
    return Clock::now();
  }
}
