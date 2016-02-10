#include "Common.h"
#include "StateMachine.h"

StateMachine::StateMachine() : mLinks(), mStates(), mCurrentState(nullptr)
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
    
  for(LinkIT it = mLinks.begin(); it != mLinks.end(); ++it)
  {
    State *start = (*it)->GetStart();
    State *end = (*it)->GetEnd();
    if(end->GetName() == aName)
    {
      if(mCurrentState == nullptr || mCurrentState == start)
      {
        mCurrentState = end;
        return;
      }
    }
  }
  assert(!"No state transfer found.");
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