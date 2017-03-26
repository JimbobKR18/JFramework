#ifndef __JFRAMEWORK_STATELINK_H
#define __JFRAMEWORK_STATELINK_H

#include "State.h"

class StateLink
{
public:
  enum LinkType
  {
    NONE,
    LEFT_RIGHT,
    RIGHT_LEFT,
    BIDIRECTIONAL
  };
  
private:
  State *mStart;
  State *mEnd;
  LinkType mType;
  
  StateLink();
public:
  StateLink(State *aStart, State *aEnd, LinkType const &aType);
  virtual ~StateLink();
  
  State *GetStart() const;
  State *GetEnd() const;
  LinkType GetType() const;
};

#endif // STATELINK_H
