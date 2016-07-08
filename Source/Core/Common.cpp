//
//  Common.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#include "Common.h"
#include "SystemProperties.h"
#include "Constants.h"

// Two macros ensures the macro passed will
// be expanded before being converted to a string.
#define STRINGIZE_DETAIL(str) #str
#define STRINGIZE(str) STRINGIZE_DETAIL(str)

namespace Common
{
  /**
   * @brief Gets relative path for asset files, set by system properties.
   * @param aSubFolder Sub folder to search inside of asset directory.
   * @param aFileName File to find (extension included)
   */
  std::string const RelativePath(std::string const &aSubFolder, std::string const &aFileName)
  {
    // Manually setting the asset directory
    // Or use defaults based on OS
    std::string ret = SystemProperties::GetAssetsDirectory();
    ret.append(aSubFolder);
    ret.append("/");
    ret.append(aFileName);
    return ret;
  }

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
   * @brief Converts String to Bool Vector
   * @param aValue
   */
  std::vector<bool> StringToBoolVector(std::string const &aValue)
  {
    std::vector<bool> ret;
    std::istringstream stream(aValue);
    
    std::string value;
    
    while(!stream.eof())
    {
      stream >> value;
      ret.push_back((value == "true" ? true : false));
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
   * @brief Converts Bool Vector to String (comma separated)
   * @param aValue
   */
  std::string BoolVectorToString(std::vector<bool> &aValue)
  {
    std::string ret;
    std::vector<bool>::iterator end = aValue.end();
    for(std::vector<bool>::iterator it = aValue.begin(); it != end; ++it)
    {
      ret += (*it ? "true" : "false");
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
   * @brief If find "Literal(" keep forming string until find ")", or Constant( "etc." )
   * @param aLiteral Literal to parse through.
   */
  std::string ParseString(std::istream *infile, std::string const &aString)
  {
    std::string ret;
    int literalLocation = aString.find("Literal");
    int constantLocation = aString.find("Constant");
    if(literalLocation == std::string::npos && constantLocation == std::string::npos)
      return aString;
    else
    {
      // Getting the full string
      int offset = (literalLocation != std::string::npos) ? literalLocation : constantLocation;
      int extraOffset = (literalLocation != std::string::npos) ? 8 : 9;
      unsigned pos = offset + extraOffset;
      char next;
      bool earlyout = false;
      // Literal(blah) is one whole word, extract
      while(pos < aString.length())
      {
        char next = aString[pos];
        if(next == '\n')
        {
          earlyout = true;
          break;
        }
        if(next != ')' && next != '(')
          ret.push_back(next);
        ++pos;
      }
      // Didn't hit newline, get rest of sentence
      while(!earlyout && infile->get(next))
      {
        if(next == '\n')
          break;
        if(next != ')' && next != '(')
          ret.push_back(next);
      }
    }
    
    if(constantLocation != std::string::npos)
      return Constants::GetString(TrimString(ret));
    return ret;
  }
  
  /**
   * @brief Trim whitespace from a string.
   * @param aString String to trim.
   * @return Trimmed string.
   */
  std::string TrimString(std::string const &aString)
  {
    std::string ret;
    for(std::string::const_iterator it = aString.begin(); it != aString.end(); ++it)
    {
      char letter = *it;
      if(letter == ' ' || letter == '\r' || letter == '\t' || letter == '\n')
      {
        continue;
      }
      else
      {
        ret.push_back(letter);
      }
    }
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
      switch(i % 6)
      {
      case 0:
        hashValue += (static_cast<int>(aString[i]) + i) ^ key[i % len];
      case 1:
        hashValue += (static_cast<int>(aString[i]) + i) & key[i % len];
      case 2:
        hashValue += (static_cast<int>(aString[i]) + i) - key[i % len];
      case 3:
        hashValue += (static_cast<int>(aString[i]) + i) + key[i % len];
      case 4:
        hashValue += (static_cast<int>(aString[i]) + i) * key[i % len];
      case 5:
        hashValue += (static_cast<int>(aString[i]) + i) / key[i % len];
      }
      switch(i % 2)
      {
      case 0:
        hashValue += aString[i] - aString[aString.size() - 1 - i];
      case 1:
        hashValue -= aString[i] + aString[aString.size() - 1 - i];
      }
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
}
