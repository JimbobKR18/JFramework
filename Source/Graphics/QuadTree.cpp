#include "QuadTree.h"
#include "GameObject.h"
#include "Transform.h"

const Vector3 MARGIN_FOR_ERROR = Vector3(128,128,128);

//----------------------------------------------
// QuadTree
//----------------------------------------------
QuadTree::QuadTree(unsigned const aCapacity, Vector3 const &aMinRange, Vector3 const &aMaxRange) : Tree(aMinRange, aMaxRange), mCapacity(aCapacity),
  mNorthWest(nullptr), mNorthEast(nullptr), mSouthWest(nullptr), mSouthEast(nullptr)
{
}

QuadTree::QuadTree(unsigned const aCapacity, Vector3 const &aMinRange, Vector3 const &aMaxRange, std::unordered_set<Renderable*> aRenderables) : Tree(aMinRange, aMaxRange), mCapacity(aCapacity), mNorthWest(nullptr), mNorthEast(nullptr), mSouthWest(nullptr), mSouthEast(nullptr)
{
  for(std::unordered_set<Renderable*>::iterator it = aRenderables.begin(); it != aRenderables.end(); ++it)
  {
    Insert(*it);
  }
}

QuadTree::~QuadTree()
{
  Clear();
}

/**
 * @brief Clear out QuadTree.
 */
void QuadTree::Clear()
{
  mObjects.clear();
  if(mNorthWest)
    delete mNorthWest;
  if(mNorthEast)
    delete mNorthEast;
  if(mSouthWest)
    delete mSouthWest;
  if(mSouthEast)
    delete mSouthEast;
  mNorthWest = nullptr;
  mNorthEast = nullptr;
  mSouthWest = nullptr;
  mSouthEast = nullptr;
}

/**
 * @brief Remove objects, resize, and replace.
 * @param aMinRange Minimum range.
 * @param aMaxRange Maximum range.
 */
void QuadTree::Resize(Vector3 const &aMinRange, Vector3 const &aMaxRange)
{
  mMinRange = aMinRange;
  mMaxRange = aMaxRange;
  std::unordered_set<Renderable*> allObjects = GetAllObjects();
  Clear();
  for(std::unordered_set<Renderable*>::iterator it = allObjects.begin(); it != allObjects.end(); ++it)
  {
    Insert(*it);
  }
}

/**
 * @brief Split up QuadTree into sections, move objects into leaves.
 */
void QuadTree::Split()
{
  Vector3 midpoint = (mMaxRange + mMinRange) / 2.0f;
  Vector3 size = mMaxRange - midpoint;
  
  mNorthWest = new QuadTree(mCapacity, mMinRange, midpoint);
  mNorthEast = new QuadTree(mCapacity, midpoint - Vector3(0, size.y, 0), midpoint + Vector3(size.x, 0, 0));
  mSouthWest = new QuadTree(mCapacity, midpoint - Vector3(size.x, 0, 0), midpoint + Vector3(0, size.y, 0));
  mSouthEast = new QuadTree(mCapacity, midpoint, mMaxRange);
  
  for(std::unordered_set<Renderable*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    bool result = mNorthWest->Insert(*it);
    if(!result)
      result = mNorthEast->Insert(*it);
    if(!result)
      result = mSouthWest->Insert(*it);
    if(!result)
      result = mSouthEast->Insert(*it);
  }
  
  // Objects should be in other nodes.
  mObjects.clear();
}

/**
 * @brief Insert object into QuadTree.
 * @param aRenderable Object to add.
 * @return True if inserted.
 */
bool QuadTree::Insert(Renderable *aRenderable)
{
  if(!ObjectInRange(aRenderable))
    return false;
    
  if(mObjects.size() < mCapacity && !mNorthWest)
  {
    mObjects.insert(aRenderable);
    return true;
  }
    
  if(!mNorthWest)
    Split();
  
  if(mNorthWest->Insert(aRenderable)) 
    return true;
  if(mNorthEast->Insert(aRenderable)) 
    return true;
  if(mSouthWest->Insert(aRenderable)) 
    return true;
  if(mSouthEast->Insert(aRenderable)) 
    return true;
    
  return false;
}

/**
 * @brief Remove object from QuadTree.
 * @param aRenderable Object to remove.
 * @return True if removed.
 */
bool QuadTree::Remove(Renderable *aRenderable)
{
  if(mObjects.find(aRenderable) != mObjects.end() && !mNorthWest)
  {
    mObjects.erase(aRenderable);
    return true;
  }
  
  if(!mNorthWest)
    return false;
    
  if(mNorthWest->Remove(aRenderable)) 
    return true;
  if(mNorthEast->Remove(aRenderable)) 
    return true;
  if(mSouthWest->Remove(aRenderable)) 
    return true;
  if(mSouthEast->Remove(aRenderable)) 
    return true;
    
  return false;
}

/**
 * @brief Get all objects inside of QuadTree and children.
 * @return All objects.
 */
std::unordered_set<Renderable*> QuadTree::GetAllObjects() const
{
  std::unordered_set<Renderable*> allObjects = mObjects;
  if(!mNorthWest)
    return allObjects;
    
  std::unordered_set<Renderable*> objects = mNorthWest->GetAllObjects();
  allObjects.insert(objects.begin(), objects.end());
  
  objects = mNorthEast->GetAllObjects();
  allObjects.insert(objects.begin(), objects.end());
  
  objects = mSouthWest->GetAllObjects();
  allObjects.insert(objects.begin(), objects.end());
  
  objects = mSouthEast->GetAllObjects();
  allObjects.insert(objects.begin(), objects.end());
  
  return allObjects;
}

/**
 * @brief Query against tree to get objects.
 * @param aMin Minimum value.
 * @param aMax Maximum value.
 * @return Objects in range.
 */
std::unordered_set<Renderable*> QuadTree::Query(Vector3 const &aMin, Vector3 const &aMax)
{
  std::unordered_set<Renderable*> ret;
  if(!SectionInRange(aMin, aMax))
    return ret;
    
  for(std::unordered_set<Renderable*>::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    if(ObjectInRange(*it))
      ret.insert(*it);
  }
  
  if(!mNorthWest)
    return ret;
  
  //Future(std::unordered_set<Renderable*>) nwTemp = ASync(&QuadTree::Query, mNorthWest, aMin, aMax);
  //Future(std::unordered_set<Renderable*>) neTemp = ASync(&QuadTree::Query, mNorthEast, aMin, aMax);
  //Future(std::unordered_set<Renderable*>) swTemp = ASync(&QuadTree::Query, mSouthWest, aMin, aMax);
  //Future(std::unordered_set<Renderable*>) seTemp = ASync(&QuadTree::Query, mSouthEast, aMin, aMax);
  
  std::unordered_set<Renderable*> const &nwResults = mNorthWest->Query(aMin, aMax);//nwTemp.get();
  std::unordered_set<Renderable*> const &neResults = mNorthEast->Query(aMin, aMax);//neTemp.get();
  std::unordered_set<Renderable*> const &swResults = mSouthWest->Query(aMin, aMax);//swTemp.get();
  std::unordered_set<Renderable*> const &seResults = mSouthEast->Query(aMin, aMax);//seTemp.get();

  ret.insert(nwResults.begin(), nwResults.end());
  ret.insert(neResults.begin(), neResults.end());
  ret.insert(swResults.begin(), swResults.end());
  ret.insert(seResults.begin(), seResults.end());
    
  mCachedQuery = ret;
  return ret;
}

/**
 * @brief Find out if an object is in the range specified.
 * @param aRenderable Object in question.
 * @return Whether the object is in this range or not.
 */
bool QuadTree::ObjectInRange(Renderable* aRenderable) const
{
  GameObject *owner = aRenderable->GetOwner();
  Transform *transform = owner->GET<Transform>();
  Vector3 midpoint = (mMaxRange + mMinRange) / 2.0f;
  Vector3 size = mMaxRange - midpoint;
  Vector3 objectPosition = transform->GetPosition();
  Vector3 objectSize = transform->GetSize();
  
  return CheckOverlap(midpoint, objectPosition, size, objectSize);
}

/**
 * @brief Find out if a section is in the range specified.
 * @param aMin Minimum range.
 * @param aMax Maximum range.
 * @return Whether the section is in this range or not.
 */
bool QuadTree::SectionInRange(Vector3 const &aMin, Vector3 const& aMax) const
{
  Vector3 midpoint = (aMax + aMin) / 2.0f;
  Vector3 size = aMax - midpoint;
  Vector3 sectionMidpoint = (mMaxRange + mMinRange) / 2.0f;
  Vector3 sectionSize = (mMaxRange - sectionMidpoint) + MARGIN_FOR_ERROR;
  
  return CheckOverlap(midpoint, sectionMidpoint, size, sectionSize);
}

/**
 * @brief Check if two boxes are overlapping.
 * @param aBox1Pos Box 1 Position
 * @param aBox2Pos Box 2 Position
 * @param aBox1Size Box 1 Size
 * @param aBox2Size Box 2 Size
 * @return Whether or not the two boxes overlap.
 */
bool QuadTree::CheckOverlap(Vector3 &aBox1Pos, Vector3 &aBox2Pos, Vector3 &aBox1Size, Vector3 &aBox2Size) const
{
  for(int i = 0; i < 2; ++i)
  {
    // Skip if objects are too small
    if(aBox1Size[i] <= 0.1f || aBox2Size[i] <= 0.1f)
    {
      continue;
    }
    if(fabs(aBox1Pos[i] - aBox2Pos[i]) > aBox1Size[i] + aBox2Size[i])
    {
      return false;
    }
  }
  return true;
}

