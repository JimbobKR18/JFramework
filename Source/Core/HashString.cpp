/*
 * HashString.cpp
 *
 *  Created on: Sep 10, 2014
 *      Author: jimbob
 */

#include "HashString.h"
#include "Common.h"
#include "LUATypes.h"
#include "MathExt.h"

// Constructors
HashString::HashString() : mString(""), mHash(0)
{
}

HashString::HashString(HashString  const &aRhs) : mString(aRhs.mString), mHash(aRhs.mHash)
{
}

HashString::HashString(char const* aString, unsigned aStart, unsigned aEnd)
{
  if(aEnd >= strlen(aString))
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

// Destructor
HashString::~HashString()
{
  mHash = -1;
  mString.clear();
}

// Const Operations
/**
 * @return The size of the string.
 */
int HashString::Size() const
{
  return mString.length();
}

/**
 * @return The size of the string.
 */
int HashString::Length() const
{
  return Size();
}

/**
 * @return Is the string empty?
 */
bool HashString::Empty() const
{
  return mString.empty();
}

/**
 * @brief Find a substring in our string.
 * @param aString The substring to search for.
 * @return If substring is in our string.
 */
bool HashString::Find(HashString const &aString) const
{
  if(aString.Length() == 0)
    assert(!"HashString is empty being passed into Find");

  return mString.find(aString.mString) != std::string::npos;
}

/**
 * @brief Get a substring out of our main string.
 * @param aStart Where the substring begins
 * @param aLength How long the substring will be
 * @return The substring
 */
HashString HashString::SubString(int aStart, int aLength) const
{
  return mString.substr(aStart, aLength);
}

/**
 * @brief Get string without whitespace.
 * @return Trimmed string.
 */
HashString HashString::Trim() const
{
  return Common::TrimString(mString);
}

/**
 * @brief Split a string based on delimiter
 * @param aDelimiter String to split by.
 * @return Vector of hashstrings.
 */
std::vector<HashString> HashString::Split(HashString const &aDelimiter) const
{
  std::vector<HashString> ret;
  unsigned nextMatch = mString.find(aDelimiter.mString);
  unsigned lastMatch = 0;
  
  // If character is first match, add blank string and move on.
  if(nextMatch == 0)
  {
    ret.push_back(HashString());
    nextMatch = mString.find(aDelimiter.mString, nextMatch+1);
  }

  // Find delimiter and split
  while(nextMatch != (unsigned)std::string::npos)
  {
    ret.push_back(HashString(mString, lastMatch, nextMatch-1));
    lastMatch = nextMatch + 1;
    nextMatch = mString.find(aDelimiter.mString, nextMatch+1);
  }
  ret.push_back(HashString(mString, lastMatch, mString.size()-1));
  return ret;
}

// Non-Const Operations
/**
 * @brief Reverse the string.
 */
void HashString::Reverse()
{
  std::reverse(mString.begin(), mString.end());
  Hash();
}

/**
 * @brief Append a character to the string.
 * @param aChar Character to append.
 */
void HashString::Push(char aChar)
{
  mString.push_back(aChar);
  Hash();
}

/**
 * @brief Clears out string.
 */
void HashString::Clear()
{
  mString.clear();
  Hash();
}

// Operators
/**
 * @brief Set string equal to another.
 * @param aRhs String to set equal to.
 */
void HashString::operator=(HashString const &aRhs)
{
  mString = aRhs.mString;
  mHash = aRhs.mHash;
}

/**
 * @brief Equality check.
 * @param aRhs String to check for equality
 * @return Whether the strings are equal.
 */
bool HashString::operator==(HashString const &aRhs) const
{
  // If the hashes aren't equal, early out.
  if(mHash != aRhs.mHash)
    return false;
  
  return mString == aRhs.mString;
}

/**
 * @brief Equality check.
 * @param aRhs String to check for equality
 * @return Whether the strings are equal.
 */
bool HashString::operator==(std::string const &aRhs) const
{
  return mString == aRhs;
}

/**
 * @brief Equality check.
 * @param aRhs String to check for equality
 * @return Whether the strings are equal.
 */
bool HashString::operator==(char const* aRhs) const
{
  return mString == aRhs;
}

/**
 * @brief Inequality check.
 * @param aRhs String to check for equality
 * @return Whether the strings are not equal.
 */
bool HashString::operator!=(HashString const &aRhs) const
{
  // If the hashes aren't equal, early out.
  if(mHash != aRhs.mHash)
    return true;
    
  return mString != aRhs.mString;
}

/**
 * @brief Inequality check.
 * @param aRhs String to check for equality
 * @return Whether the strings are not equal.
 */
bool HashString::operator!=(std::string const &aRhs) const
{
  return mString != aRhs;
}

/**
 * @brief Inequality check.
 * @param aRhs String to check for equality
 * @return Whether the strings are not equal.
 */
bool HashString::operator!=(char const* aRhs) const
{
  return mString != aRhs;
}

/**
 * @brief Check if string comes before another alphabetically
 * @param aRhs String to compare.
 * @return If this string should come before aRhs
 */
bool HashString::operator<(HashString const &aRhs) const
{
  return ComesBefore(aRhs);
}

/**
 * @brief Check if string comes before another alphabetically
 * @param aRhs String to compare.
 * @return If this string should come before aRhs
 */
bool HashString::operator<(std::string const &aRhs) const
{
  return ComesBefore(aRhs);
}

/**
 * @brief Check if string comes before another alphabetically
 * @param aRhs String to compare.
 * @return If this string should come before aRhs
 */
bool HashString::operator<(char const* aRhs) const
{
  return ComesBefore(aRhs);
}

/**
 * @brief Check if string comes after another alphabetically
 * @param aRhs String to compare.
 * @return If this string should come after aRhs
 */
bool HashString::operator>(HashString const &aRhs) const
{
  return !ComesBefore(aRhs);
}

/**
 * @brief Check if string comes after another alphabetically
 * @param aRhs String to compare.
 * @return If this string should come after aRhs
 */
bool HashString::operator>(std::string const &aRhs) const
{
  return !ComesBefore(aRhs);
}

/**
 * @brief Check if string comes after another alphabetically
 * @param aRhs String to compare.
 * @return If this string should come after aRhs
 */
bool HashString::operator>(char const* aRhs) const
{
  return !ComesBefore(aRhs);
}

/**
 * @brief Get specific character by index
 * @param aValue Index to check.
 * @return The character at index.
 */
char HashString::operator[](int aValue)
{
  return mString[aValue];
}

/**
 * @brief Conversion operator
 */
HashString::operator std::string()
{
  return mString;
}

/**
 * @brief Conversion operator
 */
HashString::operator char const*()
{
  return mString.c_str();
}

/**
 * @brief Conversion operator
 */
HashString::operator std::string() const
{
  return mString;
}

/**
 * @brief Conversion operator
 */
HashString::operator char const*() const
{
  return mString.c_str();
}

/**
 * @brief Add one string to this
 * @param aRhs The string to add
 * @return The modified string
 */
HashString HashString::operator+(HashString const &aRhs) const
{
  return HashString(mString + aRhs.mString);
}

/**
 * @brief Add one string to this
 * @param aRhs The string to add
 * @return The modified string
 */
void HashString::operator+=(HashString const &aRhs)
{
  mString += aRhs.mString;
  Hash();
}

/**
 * @brief Convert to standard string
 * @return The string as std::string
 */
std::string HashString::ToString() const
{
  return mString;
}

/**
 * @brief Convert to Char Array
 * @return Same string as Char Array
 */
char const* HashString::ToCharArray() const
{
  return mString.c_str();
}

/**
 * @brief Convert to int
 * @return String as an int, "0" is 0, "1" is 1 etc.
 */
int HashString::ToInt() const
{
  return Common::StringToInt(mString);
}

/**
 * @brief Convert to Float
 * @return String as an float, "0.0" is 0.0, "1.1" is 1.1 etc.
 */
float HashString::ToFloat() const
{
  return Common::StringToFloat(mString);
}

/**
 * @brief Convert to Bool
 * @return String as an bool, "true" is true, anything else is false
 */
bool HashString::ToBool() const
{
  return Common::StringToBool(ToLower().ToString());
}

/**
 * @brief Convert to Hash
 * @return Hash value of string
 */
int HashString::ToHash() const
{
  return mHash;
}

/**
 * @brief Convert to hashstring vector, split by ","
 * @return String as hashstring vector
 */
std::vector<HashString> HashString::ToHashStringVector() const
{
  return Common::StringToHashStringVector(mString);
}

/**
 * @brief Convert to string vector, split by ","
 * @return String as string vector
 */
std::vector<std::string> HashString::ToStringVector() const
{
  return Common::StringToStringVector(mString);
}

/**
 * @brief Convert to int vector, split by ","
 * @return String as int vector
 */
std::vector<int> HashString::ToIntVector() const
{
  return Common::StringToIntVector(mString);
}

/**
 * @brief Convert to float vector, split by ","
 * @return String as float vector
 */
std::vector<float> HashString::ToFloatVector() const
{
  return Common::StringToFloatVector(mString);
}

/**
 * @brief Convert to bool vector, split by ","
 * @return String as bool vector
 */
std::vector<bool> HashString::ToBoolVector() const
{
  return Common::StringToBoolVector(mString);
}

/**
 * @brief Convert to HashString set, split by ","
 * @return String as HashString set
 */
std::set<HashString> HashString::ToHashStringSet() const
{
  return Common::StringToHashStringSet(mString);
}

/**
 * @brief Convert to string set, split by ","
 * @return String as string set
 */
std::set<std::string> HashString::ToStringSet() const
{
  return Common::StringToStringSet(mString);
}

/**
 * @brief Convert to Vector3
 * @return String as a vector3
 */
Vector3 HashString::ToVector3() const
{
  std::vector<float> values = ToFloatVector();
  return Vector3(values[0], values[1], values[2]);
}

/**
 * @brief Convert to Vector4
 * @return String as a vector4
 */
Vector4 HashString::ToVector4() const
{
  std::vector<float> values = ToFloatVector();
  return Vector4(values[0], values[1], values[2], values[3]);
}

/**
 * @brief Convert to lower case.
 * @return The string as lower case.
 */
HashString HashString::ToLower() const
{
  HashString temp = *this;
  for(unsigned i = 0; i < mString.size(); ++i)
  {
    temp.mString[i] = std::tolower(mString[i]);
  }
  temp.Hash();
  return temp;
}

/**
 * @brief Convert to upper case.
 * @return The string as upper case.
 */
HashString HashString::ToUpper() const
{
  HashString temp = *this;
  for(unsigned i = 0; i < mString.size(); ++i)
  {
    temp.mString[i] = std::toupper(mString[i]);
  }
  temp.Hash();
  return temp;
}

/**
 * @brief Convert to literal for parsing files.
 * @return Literal(The hash string)
 */
HashString HashString::ToLiteral() const
{
  HashString literal = "Literal(";
  literal += mString;
  literal += ")";
  return literal;
}

/**
 * @brief Helper to determine is one string comes before another alphabetically.
 * @param aRhs The string to compare
 * @return Whether this string comes before aRhs.
 */
bool HashString::ComesBefore(HashString const &aRhs) const
{
  return mString.compare(aRhs.mString) < 0;
}

/**
 * @brief Hash generator helper.
 */
void HashString::Hash()
{
  mHash = Common::StringHashFunction(mString);
}

/**
* @brief Serialize to LUA
*/
void HashString::SerializeLUA()
{
  SLB::Class<HashString>("HashString")
    .constructor()
    .constructor<const char*>()
    .constructor<std::string>()
    .set("Size", &HashString::Size)
    .set("Length", &HashString::Length)
    .set("Empty", &HashString::Empty)
    .set("Find", &HashString::Find)
    .set("SubString", &HashString::SubString)
    .set("Reverse", &HashString::Reverse)
    .set("Push", &HashString::Push)
    .set("ToString", &HashString::ToString)
    .set("ToCharArray", &HashString::ToCharArray)
    .set("ToInt", &HashString::ToInt)
    .set("ToFloat", &HashString::ToFloat)
    .set("ToBool", &HashString::ToBool)
    .set("ToLower", &HashString::ToLower)
    .set("ToUpper", &HashString::ToUpper)
    .set("ToLiteral", &HashString::ToLiteral);
}

/**
 * @brief Helpful function to let HashString be used in unordered_map implementations.
 * @param hashString String to get hash from.
 * @return String hash.
 */
size_t HashString::HashStringHashFunction(HashString const &hashString)
{
  return hashString.ToHash();
}
