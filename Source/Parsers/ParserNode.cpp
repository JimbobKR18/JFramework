/*
 * Root.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: jimbob
 */

#include "ParserNode.h"

ParserNode::ParserNode() : mValue(""), mName(""), mChildren(), mParent(nullptr), mTouched(false), mRandom(false)
{
  mChildren.reserve(50);
}

ParserNode::~ParserNode()
{
  // Delete all children associated with this root
  for(parserNodeIT it = mChildren.begin(); it != mChildren.end(); ++it)
    delete *it;
  mChildren.clear();
}

/**
 * @brief Search for node whose name matches value. Does not mark nodes as touched.
 * @param aValue
 */
ParserNode const *ParserNode::Search(HashString const &aValue) const
{
  if(mName == aValue)
    return this;
  else
  {
    // Search for node of same name first, early out
    for(parserNodeConstIT it = mChildren.begin(); it != mChildren.end(); ++it)
    {
      ParserNode *ret = *it;
      if(ret->mName == aValue)
      {
        return ret;
      }
    }
  }

  return nullptr;
}

/**
 * @brief Find node whose name matches value.
 * @param aValue
 */
ParserNode *ParserNode::Find(HashString const &aValue)
{
  // Found our node
  if(mName == aValue)
  {
    mTouched = true;
    return this;
  }
  else
  {
    // Search for node of same name first, early out
    for(parserNodeIT it = mChildren.begin(); it != mChildren.end(); ++it)
    {
      ParserNode *ret = *it;
      if(ret->mName == aValue)
      {
        ret->mTouched = true;
        return ret;
      }
    }
  }

  return nullptr;
}

/**
 * @brief Find all roots at current level among children.
 * @param aValue Name of value to find.
 * @return Set of values amongst children.
 */
ParserNodeContainer ParserNode::FindAll(HashString const &aValue)
{
  ParserNodeContainer allRoots;
  
  // Search for node of same name first, early out
  for(parserNodeIT it = mChildren.begin(); it != mChildren.end(); ++it)
  {
    ParserNode *ret = *it;
    if(ret->mName == aValue)
    {
      ret->mTouched = true;
      allRoots.push_back(ret);
    }
  }
  
  return allRoots;
}

/**
 * @brief Find all nodes whose name starts with substring.
 * @param aValue Substring.
 * @return All nodes whose values meet above criteria.
 */
ParserNodeContainer ParserNode::FindAllThatStartsWith(HashString const &aValue)
{
  ParserNodeContainer allRoots;
  
  for(parserNodeIT it = mChildren.begin(); it != mChildren.end(); ++it)
  {
    ParserNode *ret = *it;
    if(ret->mName.StartsWith(aValue))
    {
      ret->mTouched = true;
      allRoots.push_back(ret);
    }
  }
  
  return allRoots;
}

/**
 * @brief Place node under this node with element name, with value.
 * @param aElement
 * @param aValue
 */
void ParserNode::Place(HashString const &aElement, HashString const &aValue)
{
  ParserNode *node = Find(aElement);

  if(!node)
  {
    node = new ParserNode();
    node->mName = aElement;
    mChildren.push_back(node);
  }
  node->mValue = aValue;
  node->mParent = this;
}

/**
 * @brief Insert root into children.
 * @param root
 */
void ParserNode::Insert(ParserNode* root)
{
  mChildren.push_back(root);
}

/**
 * @brief Reset touched status of node.
 */
void ParserNode::Reset()
{
  mTouched = false;
  for(parserNodeIT it = mChildren.begin(); it != mChildren.end(); ++it)
  {
    (*it)->Reset();
  }
}

/**
 * @brief Get name of node.
 */
HashString const& ParserNode::GetName() const
{
  return mName;
}

/**
 * @brief Get value of node
 */
HashString const& ParserNode::GetValue() const
{
  if(mRandom)
  {
    std::string ret;
    HashString subString = mValue.SubString(13, mValue.Length() - 14);
    for(int i = 0; i < subString.Length(); ++i)
      ret.push_back(subString[i]);
    
    std::vector<HashString> allOptions = HashString(ret).Split(",");
    ret = allOptions[Common::RandomIntInRange(0, allOptions.size())].ToString();
    return ret;
  }
  
  return mValue;
}

/**
 * @brief Get children for node
 */
ParserNodeContainer& ParserNode::GetChildren()
{
  return mChildren;
}

/**
 * @brief Get node parent
 */
ParserNode* ParserNode::GetParent() const
{
  return mParent;
}

/**
 * @brief Get untouched roots of node (unsearched)
 */
ParserNodeContainer ParserNode::GetUntouchedRoots() const
{
  ParserNodeContainer ret;
  for(parserNodeConstIT it = mChildren.begin(); it != mChildren.end(); ++it)
  {
    if((*it)->mTouched)
    {
      ParserNodeContainer addendum = (*it)->GetUntouchedRoots();
      ret.insert(ret.end(), addendum.begin(), addendum.end());
    }
    else
    {
      ret.push_back((*it));
      (*it)->mTouched = true;
    }
  }
  return ret;
}

/**
 * @brief Set node name
 * @param aName
 */
void ParserNode::SetName(HashString const &aName)
{
  mName = aName;
}

/**
 * @brief Set node value
 * @param aValue
 */
void ParserNode::SetValue(HashString const &aValue)
{
  if(aValue.StartsWith("RandomSelect"))
  {
    mRandom = true;
  }
  else
  {
    mRandom = false;
  }
  mValue = aValue;
}

/**
 * @brief Set node parent
 * @param aParent
 */
void ParserNode::SetParent(ParserNode* aParent)
{
  mParent = aParent;
}

/**
 * @brief Set root equal to another
 * @param rhs Root to set this equal to.
 */
void ParserNode::operator=(ParserNode const &rhs)
{
  mValue = rhs.mValue;
  mName = rhs.mName;
  mChildren = rhs.mChildren;
  mParent = rhs.mParent;
}

/**
 * @brief Simple alphabetical ordering helper.
 * @param rhs Root to check against.
 * @return True if comes before.
 */
bool ParserNode::operator<(ParserNode const &rhs)
{
  return mName < rhs.mName;
}