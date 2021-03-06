#include "StateObject.h"
#include "ObjectManager.h"

HashType const StateObject::sUID = Common::StringHashFunction("StateObject");

StateObject::StateObject() : Component(StateObject::sUID)
{
  mStateMachine = new StateMachine();
}

StateObject::StateObject(StateObject const &aStateObject) : Component(StateObject::sUID),
  mStateMachine(new StateMachine(*(aStateObject.mStateMachine)))
{
}

StateObject::~StateObject()
{
  delete mStateMachine;
}

/**
 * @brief Get state machine.
 * @return State machine.
 */
StateMachine *StateObject::GetStateMachine() const
{
  return mStateMachine;
}

/**
 * @brief Update loop.
 */
void StateObject::Update()
{
  float time = GetOwner()->GetManager()->GetOwningApp()->GetAppStep();
  mStateMachine->Update(time);
}

/**
 * @brief Send message to parent.
 * @param aMessage Message to send.
 */
void StateObject::SendMessage(Message const& aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

/**
 * @brief Receive message.
 * @param aMessage Message to parse.
 */
void StateObject::ReceiveMessage(Message const& aMessage)
{
}

/**
 * @brief Serialize to file format.
 * @param aNode ParserNode to write to.
 */
void StateObject::Serialize(ParserNode *aNode)
{
  HashString const STATE_OBJECT = "StateObject";
  StateMachine::StateContainer const& states = mStateMachine->GetStates();
  StateMachine::LinkContainer const& links = mStateMachine->GetLinks();
  int curIndex = 0;
  
  aNode->Place(STATE_OBJECT, "");
  ParserNode *stateObjectNode = aNode->Find(STATE_OBJECT);
  stateObjectNode->Place("States", "");
  stateObjectNode->Place("Links", "");
  
  ParserNode* statesNode = stateObjectNode->Find("States");
  for(StateMachine::StateIT it = states.begin(); it != states.end(); ++it, ++curIndex)
  {
    HashString name = HashString("State_") + Common::IntToString(curIndex);
    statesNode->Place(name, "");
    ParserNode* stateNode = statesNode->Find(name);
    stateNode->Place("Name", (*it)->GetName());
    stateNode->Place("TimeAlive", Common::FloatToString((*it)->GetTimeAlive()));
  }
  
  curIndex = 0;
  ParserNode* linksNode = stateObjectNode->Find("Links");
  for(StateMachine::LinkIT it = links.begin(); it != links.end(); ++it, ++curIndex)
  {
    HashString name = HashString("Link_") + Common::IntToString(curIndex);
    linksNode->Place(name, "");
    ParserNode* linkNode = linksNode->Find(name);
    HashString type = "NONE";
    switch((*it)->GetType())
    {
    case StateLink::LEFT_RIGHT:
      type = "LEFT_RIGHT";
      break;
    case StateLink::RIGHT_LEFT:
      type = "RIGHT_LEFT";
      break;
    case StateLink::BIDIRECTIONAL:
      type = "BIDIRECTIONAL";
      break;
    default:
      assert(!"Invalid link type used when serializing StateObject");
      break;
    }
    
    linkNode->Place("State_0", (*it)->GetStart()->GetName());
    linkNode->Place("State_1", (*it)->GetEnd()->GetName());
    linkNode->Place("Type", type);
  }
}

/**
 * @brief Read from format.
 * @param aParser Parser to read from.
 */
void StateObject::Deserialize(ParserNode *aNode)
{
  if(mStateMachine)
  {
    delete mStateMachine;
    mStateMachine = nullptr;
  }
  mStateMachine = new StateMachine();
  
  int curIndex = 0;
  
  std::map<HashType, State*> states;
  ParserNode* statesNode = aNode->Find("States");
  ParserNode* linksNode = aNode->Find("Links");
  
  HashString curName = HashString("State_") + Common::IntToString(curIndex);
  while(statesNode && statesNode->Find(curName))
  {
    ParserNode* stateNode = statesNode->Find(curName);
    HashString name = stateNode->Find("Name")->GetValue();
    float timeAlive = INFINITE_TIME_ALIVE;
    
    if(stateNode->Find("TimeAlive"))
    {
      timeAlive = stateNode->Find("TimeAlive")->GetValue().ToFloat();
    }
    
    State *state = new State(name, timeAlive);
    states[name.ToHash()] = state;
    
    ++curIndex;
    curName = HashString("State_") + Common::IntToString(curIndex);
  }
  
  curIndex = 0;
  curName = HashString("Link_") + Common::IntToString(curIndex);
  while(linksNode && linksNode->Find(curName))
  {
    ParserNode* linkNode = linksNode->Find(curName);
    HashString state1Name = linkNode->Find("State_0")->GetValue();
    HashString state2Name = linkNode->Find("State_1")->GetValue();
    HashString typeName = linkNode->Find("Type")->GetValue();
    
    StateLink::LinkType type = StateLink::LinkType::NONE;
    if(typeName == "LEFT_RIGHT")
      type = StateLink::LinkType::LEFT_RIGHT;
    else if(typeName == "RIGHT_LEFT")
      type = StateLink::LinkType::RIGHT_LEFT;
    else if(typeName == "BIDIRECTIONAL")
      type = StateLink::LinkType::BIDIRECTIONAL;
      
    if(states.find(state1Name.ToHash()) == states.end())
      assert(!"State 0 not found.");
    else if(states.find(state2Name.ToHash()) == states.end())
      assert(!"State 1 not found.");
    mStateMachine->AddLink(states[state1Name.ToHash()], states[state2Name.ToHash()], type);
    
    ++curIndex;
    curName = HashString("Link_") + Common::IntToString(curIndex);
  }
  
  if(aNode->Find("StartingState"))
  {
    mStateMachine->SetCurrentState(aNode->Find("StartingState")->GetValue());
  }
}

/**
 * @brief Clone StateObject
 * @param aNewOwner The new owner
 * @return Cloned StateObject
 */
Component* StateObject::Clone(GameObject *aNewOwner) const
{
  return new StateObject(*this);
}

/**
 * @brief Make this object usable in LUA
 */
void StateObject::SerializeLUA()
{
  SLB::Class<StateObject>("StateObject")
    .inherits<Component>()
    .set("GetStateMachine", &StateObject::GetStateMachine);
}
