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
  float mCurrentTime;
  
public:
  StateMachine();
  virtual ~StateMachine();
  
  void Update(float const aDT);
  bool IsCurrentStateExpired() const;
  void Reset();
  
  float GetCurrentStateTimeLeft() const;
  State* GetCurrentState();
  void SetCurrentState(HashString const &aName);
  StateContainer const& GetStates() const;
  LinkContainer const& GetLinks() const;
  
  void AddState(State* aState);
  void AddLink(StateLink *aLink);
  void AddLink(State *aState1, State *aState2, StateLink::LinkType const &aLinkType);
  
  static void SerializeLUA();
};

#endif // STATEMACHINE_H
