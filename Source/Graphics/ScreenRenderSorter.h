#ifndef SCREENRENDERSORTER_H
#define SCREENRENDERSORTER_H

#include "Common.h"
#include "Renderable.h"

// Class designed to be derived from to customize how objects are sorted.
class ScreenRenderSorter
{
public:
  ScreenRenderSorter() {}
  virtual ~ScreenRenderSorter() {}
  virtual void SortPredicate(std::vector<Renderable*> &aRenderables) = 0;
};

#endif // SCREENRENDERSORTER_H
