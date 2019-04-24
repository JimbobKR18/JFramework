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
#include "Renderable.h"
#include "Threading.h"

// Stores section data
class Tree
{
protected:
  std::unordered_set<Renderable*> mObjects;
  std::unordered_set<Renderable*> mCachedQuery;
  Vector3 mMinRange;
  Vector3 mMaxRange;
  
public:
  Tree(Vector3 const &aMinRange, Vector3 const &aMaxRange);
  virtual ~Tree();
  
  // Getters
  std::unordered_set<Renderable*> const &GetObjects() const;
  std::unordered_set<Renderable*> &GetLastQuery();
  Vector3 const &GetMinRange() const;
  Vector3 const &GetMaxRange() const;
  
  // Methods
  virtual void Clear() = 0;
  virtual void Resize(Vector3 const &aMinRange, Vector3 const &aMaxRange) = 0;
  virtual void Split() = 0;
  virtual bool Insert(Renderable *aRenderable) = 0;
  virtual bool Remove(Renderable *aRenderable) = 0;
  virtual std::unordered_set<Renderable*> GetAllObjects() const = 0;
  virtual std::unordered_set<Renderable*> Query(Vector3 const &aMin, Vector3 const &aMax) = 0;
};

#endif
