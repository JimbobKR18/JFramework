#ifndef ZRENDERSORTER_H
#define ZRENDERSORTER_H

#include "ScreenRenderSorter.h"

class ZRenderSorter : public ScreenRenderSorter
{
public:
  ZRenderSorter();
  virtual ~ZRenderSorter();
  virtual void SortPredicate(std::vector<Surface*> &aSurfaces);
};

#endif // ZRENDERSORTER_H
