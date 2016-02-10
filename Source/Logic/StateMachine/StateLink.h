#ifndef __JFRAMEWORK_STATELINK_H
#define __JFRAMEWORK_STATELINK_H

#include "State.h"

class StateLink
{
private:
  State *mStart;
  State *mEnd;
  
  StateLink();
public:
  StateLink(State *aStart, State *aEnd);
  virtual ~StateLink();
  
  State *GetStart() const;
  State *GetEnd() const;
};

#endif // STATELINK_H
