#include "Screen.h"
#include "Transform.h"
#include "ZRenderSorter.h"

bool BatchSortPredicate(Surface *object1, Surface *object2)
{
  // Simple enough
  if(object1->GetTextureID() < object2->GetTextureID())
  {
    return true;
  }

  return false;
}

Screen::Screen() : mWidth(0), mHeight(0), mRenderSorter(nullptr)
{
}

Screen::Screen(int aW, int aH) : mWidth(aW), mHeight(aH)
{
  mView.SetSize(Vector3(aW, aH, 0));
  mRenderSorter = new ZRenderSorter();
}

Screen::~Screen()
{
  delete mRenderSorter;
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

ScreenRenderSorter* Screen::GetRenderSorter()
{
  return mRenderSorter;
}

/**
 * @brief Redefine behavior of how objects should be rendered.
 * @param aRenderSorter
 */
void Screen::SetRenderSorter(ScreenRenderSorter *aRenderSorter)
{
  if(mRenderSorter)
    delete mRenderSorter;
    
  mRenderSorter = aRenderSorter;
}

/**
 * @brief Sorts objects to be rendered. (Assumes that objects of same texture ID are at the same z depth.)
 * @param aObjects
 */
void Screen::SortObjects(std::vector<Surface*> &aObjects)
{
  std::sort(aObjects.begin(), aObjects.end(), BatchSortPredicate);
  mRenderSorter->SortPredicate(aObjects);
}
