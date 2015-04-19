#ifndef BATCHRENDERSORTER_H
#define BATCHRENDERSORTER_H

#include "ScreenRenderSorter.h" // Base class: ScreenRenderSorter

class BatchRenderSorter : public ScreenRenderSorter
{
public:
  BatchRenderSorter();
  virtual ~BatchRenderSorter();
  virtual void SortPredicate(std::vector<Surface*>& aSurfaces);
};

#endif // BATCHRENDERSORTER_H
