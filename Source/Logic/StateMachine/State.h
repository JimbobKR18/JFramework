#ifndef __JFRAMEWORK_STATE_H
#define __JFRAMEWORK_STATE_H

#include "HashString.h"

class State
{
private:
  HashString mName;
  
  State();
public:
  State(HashString const &aName);
  virtual ~State();
  
  HashString const GetName() const;
  void SetName(HashString const &aName);
};

#endif // __JFRAMEWORK_STATE_H
