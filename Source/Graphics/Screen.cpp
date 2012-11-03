#include "Screen.h"

Screen::Screen() : mWidth(0), mHeight(0)
{
}

Screen::Screen(int aW, int aH) : mWidth(aW), mHeight(aH)
{
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
