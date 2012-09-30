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

int Screen::GetWidth()
{
  return mWidth;
}

int Screen::GetHeight()
{
  return mHeight;
}
