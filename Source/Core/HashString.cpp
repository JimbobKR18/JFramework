/*
 * HashString.cpp
 *
 *  Created on: Sep 10, 2014
 *      Author: jimbob
 */

#include "HashString.h"
#include "Common.h"

// Constructors
HashString::HashString() : mString(""), mHash(0)
{
}

HashString::HashString(HashString  const &aRhs) : mString(aRhs.mString), mHash(aRhs.mHash)
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

bool HashString::Empty() const
{
  return mString.empty();
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
  unsigned nextMatch = mString.find(aDelimiter.mString);
  unsigned lastMatch = 0;

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

bool HashString::operator==(HashString const &aRhs) const
{
  // If the hashes aren't equal, early out.
  if(mHash != aRhs.mHash)
    return false;
  
  return mString == aRhs.mString;
}

bool HashString::operator==(std::string const &aRhs) const
{
  return mString == aRhs;
}

bool HashString::operator==(char const* aRhs) const
{
  return mString == aRhs;
}

bool HashString::operator!=(HashString const &aRhs) const
{
  // If the hashes aren't equal, early out.
  if(mHash != aRhs.mHash)
    return true;
    
  return mString != aRhs.mString;
}

bool HashString::operator!=(std::string const &aRhs) const
{
  return mString != aRhs;
}

bool HashString::operator!=(char const* aRhs) const
{
  return mString != aRhs;
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

HashString::operator std::string() const
{
  return mString;
}

HashString::operator char const*() const
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

std::string HashString::ToString() const
{
  return mString;
}

char const* HashString::ToCharArray() const
{
  return mString.c_str();
}

int HashString::ToInt() const
{
  return Common::StringToInt(mString);
}

float HashString::ToFloat() const
{
  return Common::StringToFloat(mString);
}

bool HashString::ToBool() const
{
  return Common::StringToBool(ToLower().ToString());
}

std::vector<std::string> HashString::ToStringVector() const
{
  return Common::StringToStringVector(mString);
}

std::vector<int> HashString::ToIntVector() const
{
  return Common::StringToIntVector(mString);
}

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

HashString HashString::ToLiteral() const
{
  HashString literal = "Literal(";
  literal += mString;
  literal += ")";
  return literal;
}

void HashString::Hash()
{
  mHash = Common::StringHashFunction(mString);
}
