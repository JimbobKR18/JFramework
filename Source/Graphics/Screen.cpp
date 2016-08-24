#include "Screen.h"
#include "Transform.h"
#include "ZRenderSorter.h"
#include "BatchRenderSorter.h"

Screen::Screen() : mWidth(0), mHeight(0), mFullScreen(false), mView(), mBatchRenderSorter(nullptr), mDepthRenderSorter(nullptr)
{
}

Screen::Screen(int aW, int aH, bool aFullScreen) : mWidth(aW), mHeight(aH), mFullScreen(aFullScreen), mView()
{
  mView.SetSize(Vector3(aW, aH, 0));
  mDepthRenderSorter = new ZRenderSorter();
  mBatchRenderSorter = new BatchRenderSorter();
}

Screen::~Screen()
{
  delete mDepthRenderSorter;
  delete mBatchRenderSorter;
}

int Screen::GetWidth() const
{
  return mWidth;
}

void Screen::SetWidth(int const aWidth)
{
  mWidth = aWidth;
}

int Screen::GetHeight() const
{
  return mHeight;
}

void Screen::SetHeight(int const aHeight)
{
  mHeight = aHeight;
}

bool Screen::IsFullScreen() const
{
  return mFullScreen;
}

void Screen::SetFullScreen(bool const aFullScreen)
{
  mFullScreen = aFullScreen;
}

View &Screen::GetView()
{
  return mView;
}

ScreenRenderSorter* Screen::GetBatchRenderSorter()
{
  return mBatchRenderSorter;
}

/**
 * @brief Redefine behavior of how objects should be rendered.
 * @param aRenderSorter
 */
void Screen::SetBatchRenderSorter(ScreenRenderSorter *aBatchRenderSorter)
{
  if(mBatchRenderSorter)
    delete mBatchRenderSorter;
    
  mBatchRenderSorter = aBatchRenderSorter;
}

ScreenRenderSorter* Screen::GetDepthRenderSorter()
{
  return mDepthRenderSorter;
}

/**
 * @brief Redefine behavior of how objects should be rendered.
 * @param aRenderSorter
 */
void Screen::SetDepthRenderSorter(ScreenRenderSorter *aDepthRenderSorter)
{
  if(mDepthRenderSorter)
    delete mDepthRenderSorter;
    
  mDepthRenderSorter = aDepthRenderSorter;
}

/**
 * @brief Sorts objects to be rendered. (Assumes that objects of same texture ID are at the same z depth.)
 * @param aObjects
 */
void Screen::SortObjects(std::vector<Surface*> &aObjects)
{
  mBatchRenderSorter->SortPredicate(aObjects);
  mDepthRenderSorter->SortPredicate(aObjects);
}