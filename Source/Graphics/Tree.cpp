//
//  Tree.cpp
//  Chang-e
//
//  Created by Jimmy Spencer on 12/6/18.
//  Copyright © 2018 Jimmy Spencer. All rights reserved.
//

#include "Tree.h"

Tree::Tree(Vector3 const &aMinRange, Vector3 const &aMaxRange) : mObjects(), mCachedQuery(),
mMinRange(aMinRange), mMaxRange(aMaxRange)
{
}

Tree::~Tree()
{
}

/**
 * @brief Get objects in this section.
 * @return The objects in the section.
 */
std::unordered_set<Surface*> const &Tree::GetObjects() const
{
  return mObjects;
}

/**
 * @brief Get results of last query.
 * @return Last query results.
 */
std::unordered_set<Surface*> &Tree::GetLastQuery()
{
  return mCachedQuery;
}

/**
 * @brief Get minimum range of section.
 * @return The minimum range of the section.
 */
Vector3 const &Tree::GetMinRange() const
{
  return mMinRange;
}

/**
 * @brief Get maximum range of section.
 * @return The maximum range of the section.
 */
Vector3 const &Tree::GetMaxRange() const
{
  return mMaxRange;
}
