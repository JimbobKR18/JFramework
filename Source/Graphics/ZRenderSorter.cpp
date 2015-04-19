#include "ZRenderSorter.h"
#include "Transform.h"

bool ZSortPredicate(Surface *object1, Surface *object2)
{
  // Default values in case no owner found.
  float z1 = -0.9999f;
  float z2 = -0.9999f;
  int id1 = 0;
  int id2 = 0;
  
  if(object1->GetOwner())
  {
    z1 = object1->GetOwner()->GET<Transform>()->GetPosition().z;
    id1 = object1->GetTextureID();
  }
  if(object2->GetOwner())
  {
    z2 = object2->GetOwner()->GET<Transform>()->GetPosition().z;
    id2 = object2->GetTextureID();
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
  
  return false;
}

ZRenderSorter::ZRenderSorter()
{
}

ZRenderSorter::~ZRenderSorter()
{
}

void ZRenderSorter::SortPredicate(std::vector<Surface*> &aSurfaces)
{
  std::sort(aSurfaces.begin(), aSurfaces.end(), ZSortPredicate);
}