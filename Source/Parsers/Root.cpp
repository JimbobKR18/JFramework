/*
 * Root.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: jimbob
 */

#include "Root.h"

Root::Root() : mValue(""), mName(""), mChildren(), mParent(NULL), mTouched(false)
{
}

Root::~Root()
{
  // Delete all children associated with this root
  for(rootIT it = mChildren.begin(); it != mChildren.end(); ++it)
      delete *it;
}

Root const *Root::Search(std::string const &aValue) const
{
  if(mName.ToString() == aValue)
    return this;
  else
  {
    // Search children for node
    for(rootConstIT it = mChildren.begin(); it != mChildren.end(); ++it)
    {
      Root const *ret = (*it)->Search(aValue);
      if(ret)
      {
        if(ret->mName.ToString() == aValue)
        {
          return ret;
        }
      }
    }
  }

  return NULL;
}


Root *Root::Find(std::string const &aValue)
{
  // Found our node
  if(mName == aValue)
  {
    mTouched = true;
    return this;
  }
  else
  {
    // Search children for node
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

  return NULL;
}

void Root::Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue)
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
    return;
  }
  else
  {
    if(mName == aElement)
    {
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
}

void Root::Insert(Root* root)
{
  mChildren.insert(root);
}

HashString Root::GetName() const
{
  return mName;
}

HashString Root::GetValue() const
{
  return mValue;
}

RootContainer& Root::GetChildren()
{
  return mChildren;
}

Root* Root::GetParent() const
{
  return mParent;
}

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


void Root::SetName(HashString const &aName)
{
  mName = aName;
}

void Root::SetValue(HashString const &aValue)
{
  mValue = aValue;
}

void Root::SetParent(Root* aParent)
{
  mParent = aParent;
}
