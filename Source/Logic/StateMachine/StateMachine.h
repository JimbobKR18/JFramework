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
  
  enum LinkType
  {
    LEFT_RIGHT,
    RIGHT_LEFT,
    BIDIRECTIONAL
  };
  
private:
  LinkContainer mLinks;
  StateContainer mStates;
  State* mCurrentState;
  float mCurrentTime;
  
public:
  StateMachine();
  virtual ~StateMachine();
  
  void Update(float const aDT);
  bool IsCurrentStateExpired() const;
  
  State* GetCurrentState() const;
  void SetCurrentState(HashString const &aName);
  
  void AddLink(StateLink *aLink);
  void AddLink(State *aState1, State *aState2, LinkType const &aLinkType);
};

#endif // STATEMACHINE_H
