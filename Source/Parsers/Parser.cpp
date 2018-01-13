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
ParserNode* Parser::GetBaseRoot() const
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
ParserNode* Parser::Find(HashString const &aElement)
{
  // Search inside roots to find value
  ParserNode *value = mDictionary->Find(aElement);
  return value;
}

/**
 * @brief Find node by name two levels deep (first parent, then element)
 * @param aRoot
 * @param aElement
 */
ParserNode* Parser::Find(HashString const &aRoot, HashString const &aElement)
{
  // Find node and search it for an element
  std::vector<HashString> splitString = aRoot.Split("/");
  ParserNode *node = mDictionary->Find(splitString[0]);

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
std::set<ParserNode*> Parser::FindAll(HashString const &aElement)
{
  // Find node and search it for an element
  ParserNode *node = mDictionary->Find(aElement);
  return node->FindAll(aElement);
}

/**
 * @brief Write value to element
 * @param aElement
 * @param aValue
 */
void Parser::Place(HashString const &aElement, HashString const &aValue)
{
  ParserNode* newRoot = SetUpTree(mDictionary, aElement.Split("/"));
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
  ParserNode* newNodes = SetUpTree(mDictionary, aRoot.Split("/"));
  ParserNode* newRoot = SetUpTree(newNodes, aElement.Split("/"));
  newRoot->SetValue(aValue);
}

/**
 * @brief From root, build tree, return lowest level node creating
 * @param aBase Where to start building from.
 * @param aStringHierarchy
 */
ParserNode* Parser::SetUpTree(ParserNode* aBase, std::vector<HashString> const &aStringHierarchy)
{
  ParserNode* current = aBase;
  ParserNode* ret = nullptr;
  
  for(unsigned i = 0; i < aStringHierarchy.size(); ++i)
  {
    ParserNode* next = current->Find(aStringHierarchy[i]);
    if(!next)
    {
      next = new ParserNode();
      current->Insert(next);
      next->SetParent(current);
      next->SetName(aStringHierarchy[i]);
    }
    ret = next;
    current = next;
  }
  
  return ret;
}