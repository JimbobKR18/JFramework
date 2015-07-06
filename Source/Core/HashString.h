/*
 * HashString.h
 *
 *  Created on: Sep 10, 2014
 *      Author: jimbob
 */

#ifndef HASHSTRING_H_
#define HASHSTRING_H_

#include <string>
#include <vector>

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
  HashString(HashString  const &aRhs);
  HashString(char const* aString, unsigned aStart = 0, unsigned aEnd = 0);
  HashString(std::string const &aString, unsigned aStart = 0, unsigned aEnd = 0);

  // Const Operations
  int Size() const;
  int Length() const;
  bool Empty() const;
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
  bool operator==(HashString const &aRhs) const;
  bool operator==(std::string const &aRhs) const;
  bool operator==(char const* aRhs) const;
  bool operator!=(HashString const &aRhs) const;
  bool operator!=(std::string const &aRhs) const;
  bool operator!=(char const* aRhs) const;
  char operator[](int aValue);
  // Conversion
  operator std::string();
  operator char const*();
  operator std::string() const;
  operator char const*() const;
  // Addition
  HashString operator+(HashString const &aRhs) const;
  void operator+=(HashString const &aRhs);
  // Misc.
  std::string ToString() const;
  char const* ToCharArray() const;
  int ToInt() const;
  float ToFloat() const;
  bool ToBool() const;
  std::vector<std::string> ToStringVector() const;
  std::vector<int> ToIntVector() const;
  HashString ToLower() const;
  HashString ToUpper() const;
  HashString ToLiteral() const;
private:
  void Hash();
};

#endif /* HASHSTRING_H_ */
