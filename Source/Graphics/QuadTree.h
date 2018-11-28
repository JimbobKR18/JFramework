#ifndef QUADTREE_H
#define QUADTREE_H

#include "Common.h"
#include "MathExt.h"
#include "Surface.h"

// Stores section data
class QuadTree
{
private:
  unsigned mCapacity;

  std::unordered_set<Surface*> mObjects;
  Vector3 mMinRange;
  Vector3 mMaxRange;
  
  QuadTree *mNorthWest;
  QuadTree *mNorthEast;
  QuadTree *mSouthWest;
  QuadTree *mSouthEast;
  
public:
  QuadTree(unsigned const aCapacity, Vector3 const &aMinRange, Vector3 const &aMaxRange);
  QuadTree(unsigned const aCapacity, Vector3 const &aMinRange, Vector3 const &aMaxRange, std::unordered_set<Surface*> aObjects);
  virtual ~QuadTree();
  
  // Getters
  std::unordered_set<Surface*> const &GetObjects() const;
  Vector3 const &GetMinRange() const;
  Vector3 const &GetMaxRange() const;
  
  // Methods
  void Clear();
  void Resize(Vector3 const &aMinRange, Vector3 const &aMaxRange);
  void Split();
  bool Insert(Surface *aSurface);
  bool Remove(Surface *aSurface);
  std::unordered_set<Surface*> GetAllObjects() const;
  std::unordered_set<Surface*> Query(Vector3 const &aMin, Vector3 const &aMax) const;
  
private:
  bool ObjectInRange(Surface* aSurface) const;
  bool SectionInRange(Vector3 const &aMin, Vector3 const& aMax) const;
  bool CheckOverlap(Vector3 &aBox1Pos, Vector3 &aBox2Pos, Vector3 &aBox1Size, Vector3 &aBox2Size) const;
};

#endif // QUADTREE_H
