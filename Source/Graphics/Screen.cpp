#include "Screen.h"
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

bool BatchSortPredicate(Surface *object1, Surface *object2)
{
  // Simple enough
  if(object1->GetTextureID() < object2->GetTextureID())
  {
    return true;
  }

  return false;
}

Screen::Screen() : mWidth(0), mHeight(0)
{
}

Screen::Screen(int aW, int aH) : mWidth(aW), mHeight(aH)
{
  mView.SetSize(Vector3(aW, aH, 0));
}

Screen::~Screen()
{

}

int Screen::GetWidth() const
{
  return mWidth;
}

int Screen::GetHeight() const
{
  return mHeight;
}

View &Screen::GetView()
{
	return mView;
}

// This assumes that objects of same texture ID are at the same z depth.
void Screen::SortObjects(std::vector<Surface*> &aObjects)
{
  std::sort(aObjects.begin(), aObjects.end(), BatchSortPredicate);
  std::sort(aObjects.begin(), aObjects.end(), ZSortPredicate);
}
