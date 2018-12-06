//
//  Tree.hpp
//  Chang-e
//
//  Created by Jimmy Spencer on 12/6/18.
//  Copyright © 2018 Jimmy Spencer. All rights reserved.
//

#ifndef __JFramework_Tree_h_
#define __JFramework_Tree_h_

#include "Common.h"
#include "MathExt.h"
#include "Surface.h"
#include "Threading.h"

// Stores section data
class Tree
{
protected:
  std::unordered_set<Surface*> mObjects;
  std::unordered_set<Surface*> mCachedQuery;
  Vector3 mMinRange;
  Vector3 mMaxRange;
  
public:
  Tree(Vector3 const &aMinRange, Vector3 const &aMaxRange);
  virtual ~Tree();
  
  // Getters
  std::unordered_set<Surface*> const &GetObjects() const;
  std::unordered_set<Surface*> &GetLastQuery();
  Vector3 const &GetMinRange() const;
  Vector3 const &GetMaxRange() const;
  
  // Methods
  virtual void Clear() = 0;
  virtual void Resize(Vector3 const &aMinRange, Vector3 const &aMaxRange) = 0;
  virtual void Split() = 0;
  virtual bool Insert(Surface *aSurface) = 0;
  virtual bool Remove(Surface *aSurface) = 0;
  virtual std::unordered_set<Surface*> GetAllObjects() const = 0;
  virtual std::unordered_set<Surface*> Query(Vector3 const &aMin, Vector3 const &aMax) = 0;
};

#endif
