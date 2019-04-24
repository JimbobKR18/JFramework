#ifndef QUADTREE_H
#define QUADTREE_H

#include "Common.h"
#include "MathExt.h"
#include "Renderable.h"
#include "Tree.h"

// Stores section data
class QuadTree : public Tree
{
private:
  unsigned mCapacity;
  
  QuadTree *mNorthWest;
  QuadTree *mNorthEast;
  QuadTree *mSouthWest;
  QuadTree *mSouthEast;
  
public:
  QuadTree(unsigned const aCapacity, Vector3 const &aMinRange, Vector3 const &aMaxRange);
  QuadTree(unsigned const aCapacity, Vector3 const &aMinRange, Vector3 const &aMaxRange, std::unordered_set<Renderable*> aObjects);
  virtual ~QuadTree();
  
  // Methods
  virtual void Clear();
  virtual void Resize(Vector3 const &aMinRange, Vector3 const &aMaxRange);
  virtual void Split();
  virtual bool Insert(Renderable *aRenderable);
  virtual bool Remove(Renderable *aRenderable);
  virtual std::unordered_set<Renderable*> GetAllObjects() const;
  virtual std::unordered_set<Renderable*> Query(Vector3 const &aMin, Vector3 const &aMax);
  
private:
  bool ObjectInRange(Renderable* aRenderable) const;
  bool SectionInRange(Vector3 const &aMin, Vector3 const& aMax) const;
  bool CheckOverlap(Vector3 &aBox1Pos, Vector3 &aBox2Pos, Vector3 &aBox1Size, Vector3 &aBox2Size) const;
};

#endif // QUADTREE_H
