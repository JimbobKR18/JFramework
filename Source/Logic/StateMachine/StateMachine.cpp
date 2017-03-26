#include "Common.h"
#include "StateMachine.h"

StateMachine::StateMachine() : mLinks(), mStates(), mCurrentState(nullptr), mCurrentTime(0)
{
}

StateMachine::~StateMachine()
{
  for(LinkIT it = mLinks.begin(); it != mLinks.end(); ++it)
  {
    delete *it;
  }
  for(StateIT it = mStates.begin(); it != mStates.end(); ++it)
  {
    delete *it;
  }
}

/**
 * @brief Update state machine.
 * @param aDT Amount of time that has passed.
 */
void StateMachine::Update(float const aDT)
{
  mCurrentTime += aDT;
}

/**
 * @brief Get time left in current state.
 * @return Time left as float.
 */
float StateMachine::GetCurrentStateTimeLeft() const
{
  if(!mCurrentState || mCurrentState->GetTimeAlive() <= 0.0f)
    return 0;
    
  return mCurrentState->GetTimeAlive() - mCurrentTime;
}

/**
 * @brief Check if current state is expired.
 * @return True if expired.
 */
bool StateMachine::IsCurrentStateExpired() const
{
  if(!mCurrentState || mCurrentState->GetTimeAlive() <= 0.0f)
    return false;
    
  return mCurrentState->GetTimeAlive() <= mCurrentTime;
}

/**
 * @brief Get current state of StateMachine
 * @return Current State
 */
State* StateMachine::GetCurrentState() const
{
  return mCurrentState;
}

/**
 * @brief Set current state, if no link from current state to end
 *          state is found, an assert is made.
 * @param aName Name of state.
 */
void StateMachine::SetCurrentState(HashString const &aName)
{
  if(mCurrentState && mCurrentState->GetName() == aName)
    return;
    
  mCurrentTime = 0;
  for(LinkIT it = mLinks.begin(); it != mLinks.end(); ++it)
  {
    State *start = (*it)->GetStart();
    State *end = (*it)->GetEnd();
    if(end->GetName() == aName && 
      ((*it)->GetType() == StateLink::LinkType::LEFT_RIGHT || (*it)->GetType() == StateLink::LinkType::BIDIRECTIONAL))
    {
      if(mCurrentState == nullptr || mCurrentState == start)
      {
        mCurrentState = end;
        return;
      }
    }
    else if(start->GetName() == aName && 
      ((*it)->GetType() == StateLink::LinkType::RIGHT_LEFT || (*it)->GetType() == StateLink::LinkType::BIDIRECTIONAL))
    {
      if(mCurrentState == nullptr || mCurrentState == end)
      {
        mCurrentState = start;
        return;
      }
    }
  }
  assert(!"No state transfer found.");
}

/**
 * @brief Get states. Unmutable.
 * @return States.
 */
StateMachine::StateContainer const& StateMachine::GetStates() const
{
  return mStates;
}

/**
 * @brief Get links. Unmutable.
 * @return Links.
 */
StateMachine::LinkContainer const& StateMachine::GetLinks() const
{
  return mLinks;
}

/**
 * @brief Add link to state machine.
 * @param aLink Link
 */
void StateMachine::AddLink(StateLink *aLink)
{
  mLinks.push_back(aLink);
  mStates.insert(aLink->GetStart());
  mStates.insert(aLink->GetEnd());
}

/**
 * @brief Add link to state machine.
 * @param aState1 State 1
 * @param aState2 State 2
 * @param aLinkType How these should be linked.
 *      State1 -> State2 (LEFT_RIGHT)
 *      State2 -> State1 (RIGHT_LEFT)
 *      State1 <-> State2 (BIDIRECTIONAL)
 */
void StateMachine::AddLink(State *aState1, State *aState2, StateLink::LinkType const &aLinkType)
{
  mStates.insert(aState1);
  mStates.insert(aState2);
  
  switch(aLinkType)
  {
    case StateLink::LEFT_RIGHT:
      mLinks.push_back(new StateLink(aState1, aState2, aLinkType));
      break;
    case StateLink::RIGHT_LEFT:
      mLinks.push_back(new StateLink(aState2, aState1, aLinkType));
      break;
    case StateLink::BIDIRECTIONAL:
      mLinks.push_back(new StateLink(aState1, aState2, aLinkType));
      break;
    default:
      assert(!"How did you even get here?");
      break;
  }
}