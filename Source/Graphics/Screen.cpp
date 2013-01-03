#include "Screen.h"

bool SortPredicate(Surface *object1, Surface *object2)
{
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

std::vector<Surface*> Screen::SortObjects(std::vector<Surface*> const &aObjects)
{
  std::vector<Surface*> ret = aObjects;
  std::sort(ret.begin(), ret.end(), SortPredicate);
  return ret;
}
