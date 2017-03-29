/*
 * Root.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: jimbob
 */

#include "Root.h"

Root::Root() : mValue(""), mName(""), mChildren(), mParent(nullptr), mTouched(false)
{
}

Root::~Root()
{
  // Delete all children associated with this root
  for(rootIT it = mChildren.begin(); it != mChildren.end(); ++it)
      delete *it;
  mChildren.clear();
}

/**
 * @brief Search for node whose name matches value. Does not mark nodes as touched.
 * @param aValue
 */
Root const *Root::Search(HashString const &aValue) const
{
  if(mName == aValue)
    return this;
  else
  {
    // Search for node of same name first, early out
    for(rootIT it = mChildren.begin(); it != mChildren.end(); ++it)
    {
      Root *ret = *it;
      if(ret->mName == aValue)
      {
        return ret;
      }
    }
    // Search children for node (deeper)
    for(rootConstIT it = mChildren.begin(); it != mChildren.end(); ++it)
    {
      Root const *ret = (*it)->Search(aValue);
      if(ret)
      {
        if(ret->mName == aValue)
        {
          return ret;
        }
      }
    }
  }

  return nullptr;
}

/**
 * @brief Find node whose name matches value.
 * @param aValue
 */
Root *Root::Find(HashString const &aValue)
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
    for(rootIT it = mChildren.begin(); it != mChildren.end(); ++it)
    {
      Root *ret = *it;
      if(ret->mName == aValue)
      {
        ret->mTouched = true;
        return ret;
      }
    }
    // Search children for node (deeper)
    for(rootIT it = mChildren.begin(); it != mChildren.end(); ++it)
    {
      Root *ret = (*it)->Find(aValue);
      if(ret)
      {
        if(ret->mName == aValue)
        {
          ret->mTouched = true;
          return ret;
        }
      }
    }
  }

  return nullptr;
}

/**
 * @brief Place node under this node with root and element name, with value.
 * @param aRoot
 * @param aElement
 * @param aValue
 */
void Root::Place(HashString const &aRoot, HashString const &aElement, HashString const &aValue)
{
  if(mName == aRoot)
  {
    Root *node = Find(aElement);

    if(!node)
    {
      node = new Root();
      node->mName = aElement;
    }
    node->mValue = aValue;
    node->mParent = this;
    mChildren.insert(node);
  }
  else if(mName == aElement)
  {
    DebugLogPrint("info: Value %s found in %s, old value: %s new value: %s\n", 
      aElement.ToCharArray(), aRoot.ToCharArray(), mValue.ToCharArray(), aValue.ToCharArray());
    mValue = aValue;
  }
  else
  {
    for(rootIT it = mChildren.begin(); it != mChildren.end(); ++it)
    {
      (*it)->Place(aRoot, aElement, aValue);
    }
  }
}

/**
 * @brief Insert root into children.
 * @param root
 */
void Root::Insert(Root* root)
{
  mChildren.insert(root);
}

/**
 * @brief Get name of node.
 */
HashString Root::GetName() const
{
  return mName;
}

/**
 * @brief Get value of node
 */
HashString Root::GetValue() const
{
  return mValue;
}

/**
 * @brief Get children for node
 */
RootContainer& Root::GetChildren()
{
  return mChildren;
}

/**
 * @brief Get node parent
 */
Root* Root::GetParent() const
{
  return mParent;
}

/**
 * @brief Get untouched roots of node (unsearched)
 */
RootContainer Root::GetUntouchedRoots() const
{
  RootContainer ret;
  for(rootIT it = mChildren.begin(); it != mChildren.end(); ++it)
  {
    if((*it)->mTouched)
    {
      RootContainer addendum = (*it)->GetUntouchedRoots();
      ret.insert(addendum.begin(), addendum.end());
    }
    else
    {
      ret.insert((*it));
      (*it)->mTouched = true;
    }
  }
  return ret;
}

/**
 * @brief Set node name
 * @param aName
 */
void Root::SetName(HashString const &aName)
{
  mName = aName;
}

/**
 * @brief Set node value
 * @param aValue
 */
void Root::SetValue(HashString const &aValue)
{
  mValue = aValue;
}

/**
 * @brief Set node parent
 * @param aParent
 */
void Root::SetParent(Root* aParent)
{
  mParent = aParent;
}

/**
 * @brief Set root equal to another
 * @param rhs Root to set this equal to.
 */
void Root::operator=(Root const &rhs)
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
bool Root::operator<(Root const &rhs)
{
  return mName < rhs.mName;
}