#include "Parser.h"

Parser::Parser(HashString const &aFilename) : mCurrentObjectIndex(0),
                                               mFilename(aFilename), mDictionary(nullptr)
{
}
Parser::~Parser()
{
  delete mDictionary;
}

/**
 * @brief Get current object index (which object are we looking at?
 */
int Parser::GetCurrentObjectIndex() const
{
  return mCurrentObjectIndex;
}

/**
 * @brief Get name of file.
 */
std::string Parser::GetFilename() const
{
  return mFilename;
}

/**
 * @brief Get top level root node.
 */
Root* Parser::GetBaseRoot() const
{
  return mDictionary;
}

/**
 * @brief Set object index.
 * @param aIndex
 */
void Parser::SetCurrentObjectIndex(int const aIndex)
{
  mCurrentObjectIndex = aIndex;
}

/**
 * @brief Find element by name (first occurrence)
 * @param aElement Element string to find
 */
Root* Parser::Find(HashString const &aElement)
{
  // Search inside roots to find value
  Root *value = mDictionary->Find(aElement);
  return value;
}

/**
 * @brief Find node by name two levels deep (first parent, then element)
 * @param aRoot
 * @param aElement
 */
Root* Parser::Find(HashString const &aRoot, HashString const &aElement)
{
  // Find node and search it for an element
  std::vector<HashString> splitString = aRoot.Split("/");
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

/**
 * @brief Find all nodes with matching name.
 * @param aElement Name of element.
 * @return All elements matching name.
 */
std::set<Root*> Parser::FindAll(HashString const &aElement)
{
  // Find node and search it for an element
  Root *node = mDictionary->Find(aElement);
  return node->FindAll(aElement);
}

/**
 * @brief Write value to element
 * @param aElement
 * @param aValue
 */
void Parser::Place(HashString const &aElement, HashString const &aValue)
{
  Root* newRoot = SetUpTree(mDictionary, aElement.Split("/"));
  newRoot->SetValue(aValue);
}

/**
 * @brief Write value to element under parent node.
 * @param aRoot
 * @param aElement
 * @param aValue
 */
void Parser::Place(HashString const &aRoot, HashString const &aElement, HashString const &aValue)
{
  Root* newNodes = SetUpTree(mDictionary, aRoot.Split("/"));
  Root* newRoot = SetUpTree(newNodes, aElement.Split("/"));
  newRoot->SetValue(aValue);
}

/**
 * @brief From root, build tree, return lowest level node creating
 * @param aBase Where to start building from.
 * @param aStringHierarchy
 */
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