#ifndef __JFRAMEWORK_STATEMACHINE_H
#define __JFRAMEWORK_STATEMACHINE_H

#include "StateLink.h"

class StateMachine
{
public:
  typedef std::vector<StateLink*> LinkContainer;
  typedef LinkContainer::const_iterator LinkIT;
  typedef std::set<State*> StateContainer;
  typedef StateContainer::const_iterator StateIT;
  
private:
  LinkContainer mLinks;
  StateContainer mStates;
  State* mCurrentState;
  
public:
  StateMachine();
  virtual ~StateMachine();
  
  State* GetCurrentState() const;
  
  void SetCurrentState(HashString const &aName);
  void AddLink(StateLink *aLink);
};

#endif // STATEMACHINE_H
