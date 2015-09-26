#include "Parser.h"

Parser::Parser(std::string const &aFilename) : mCurrentObjectIndex(0),
                                               mFilename(aFilename), mDictionary(nullptr)
{
}
Parser::~Parser()
{
  delete mDictionary;
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
  std::vector<HashString> splitString = HashString(aRoot).Split("/");
  Root *node = mDictionary->Find(splitString[0]);

  if(!node)
    return nullptr;

  for(unsigned curLevel = 0; curLevel < splitString.size(); ++curLevel)
  {
    node = node->Find(splitString[curLevel]);
    if(!node)
      return nullptr;
  }

  node = node->Find(aElement);
  return node;
}

// Writing
void Parser::Place(std::string const &aElement, std::string const &aValue)
{
  Root* newRoot = SetUpTree(mDictionary, HashString(aElement).Split("/"));
  newRoot->SetValue(aValue);
}

void Parser::Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue)
{
  Root* newNodes = SetUpTree(mDictionary, HashString(aRoot).Split("/"));
  Root* newRoot = SetUpTree(newNodes, HashString(aElement).Split("/"));
  newRoot->SetValue(aValue);
}

// Returns the deepest node.
Root* Parser::SetUpTree(Root* aBase, std::vector<HashString> const &aStringHierarchy)
{
  Root* current = aBase;
  Root* ret = nullptr;
  
  for(unsigned i = 0; i < aStringHierarchy.size(); ++i)
  {
    Root* next = current->Find(aStringHierarchy[i]);
    if(!next)
    {
      next = new Root();
      current->Insert(next);
      next->SetParent(current);
      next->SetName(aStringHierarchy[i]);
    }
    ret = next;
    current = next;
  }
  
  return ret;
}