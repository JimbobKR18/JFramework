#include "ZRenderSorter.h"
#include "Transform.h"

/**
 * @brief Sorts objects by Z axis
 * @param object1
 * @param object2
 * @return true if object1 goes before object2, false otherwise.
 */
bool ZSortPredicate(Surface *object1, Surface *object2)
{
  // Default values in case no owner found.
  float z1 = -0.9999f;
  float z2 = -0.9999f;
  int id1 = 0;
  int id2 = 0;
  int p1 = 0;
  int p2 = 0;
  
  if(object1->GetOwner())
  {
    z1 = object1->GetOwner()->GET<Transform>()->GetHierarchicalPosition().z;
    id1 = object1->GetTextureID();
    p1 = object1->GetProgramID();
  }
  if(object2->GetOwner())
  {
    z2 = object2->GetOwner()->GET<Transform>()->GetHierarchicalPosition().z;
    id2 = object2->GetTextureID();
    p2 = object2->GetProgramID();
  }

  // Right hand rule
  if(z1 < z2)
  {
    return true;
  }
  else if(z1 == z2 && id1 < id2)
  {
    return true;
  }
  else if(z1 == z2 && id1 == id2 && p1 < p2)
  {
    return true;
  }
  
  return false;
}

ZRenderSorter::ZRenderSorter()
{
}

ZRenderSorter::~ZRenderSorter()
{
}

/**
 * @brief Sorts a list of surfaces.
 * @param aSurfaces
 */
void ZRenderSorter::SortPredicate(std::vector<Surface*> &aSurfaces)
{
  std::sort(aSurfaces.begin(), aSurfaces.end(), ZSortPredicate);
}