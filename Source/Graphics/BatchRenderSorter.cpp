#include "BatchRenderSorter.h"

/**
 * @brief Sorts objects by texture ID
 * @param object1
 * @param object2
 * @return true if object1 goes before object2, false oterhwise.
 */
bool BatchSortPredicate(Surface *object1, Surface *object2)
{
  // Simple enough
  if(object1->GetTextureID() < object2->GetTextureID())
  {
    return true;
  }
  else if(object1->GetTextureID() == object2->GetTextureID() && object1->GetProgramID() < object2->GetProgramID())
  {
    return true;
  }

  return false;
}

BatchRenderSorter::BatchRenderSorter()
{
}

BatchRenderSorter::~BatchRenderSorter()
{
}

/**
 * @brief Sorts a list of surfaces.
 * @param aSurfaces
 */
void BatchRenderSorter::SortPredicate(std::vector<Surface*>& aSurfaces)
{
  std::sort(aSurfaces.begin(), aSurfaces.end(), BatchSortPredicate);
}
