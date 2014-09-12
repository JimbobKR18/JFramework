#include "Parser.h"

Parser::Parser(std::string const &aFilename) : mCurrentObjectIndex(0),
                                               mFilename(aFilename), mDictionary(NULL)
{

}
Parser::~Parser()
{

}

// Getters
int Parser::GetCurrentObjectIndex() const
{
  return mCurrentObjectIndex;
}

std::string Parser::GetFilename() const
{
  return mFilename;
}

Root* Parser::GetBaseRoot() const
{
  return mDictionary;
}

// Setters
void Parser::SetCurrentObjectIndex(int const aIndex)
{
  mCurrentObjectIndex = aIndex;
}

// Reading
Root* Parser::Find(std::string const &aElement)
{
  // Search inside roots to find value
  Root *value = mDictionary->Find(aElement);
  return value;
}

Root* Parser::Find(std::string const &aRoot, std::string const &aElement)
{
  // Find node and search it for an element
  unsigned curLevel = 0;
  std::vector<HashString> splitString = HashString(aRoot).Split("/");
  Root *node = mDictionary->Find(splitString[0]);

  if(!node)
    return nullptr;

  while(curLevel < splitString.size() - 1)
  {
    node = node->Find(splitString[curLevel]);
    ++curLevel;
    if(!node)
      return nullptr;
  }

  node = node->Find(aElement);
  return node;
}

// Writing
void Parser::Place(std::string const &aElement, std::string const &aValue)
{
  Root* newRoot = mDictionary->Find(aElement);
  if(!newRoot)
  {
    newRoot = new Root();
    mDictionary->Insert(newRoot);
    newRoot->SetParent(mDictionary);
  }
  newRoot->SetName(aElement);
  newRoot->SetValue(aValue);
}

void Parser::Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue)
{
  Root* newRoot = mDictionary->Find(aRoot)->Find(aElement);
  if(!newRoot)
  {
    newRoot = new Root();
    mDictionary->Find(aRoot)->Insert(newRoot);
    newRoot->SetParent(mDictionary->Find(aRoot));
  }
  newRoot->SetName(aElement);
  newRoot->SetValue(aValue);
}
