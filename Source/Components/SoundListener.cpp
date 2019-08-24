#include "SoundListener.h"
#include "LUATypes.h"

HashType const SoundListener::sUID = Common::StringHashFunction("SoundListener");

SoundListener::SoundListener() : Component(SoundListener::sUID), mActive(true)
{
}

SoundListener::SoundListener(SoundListener const &aSoundListener) : Component(SoundListener::sUID), mActive(aSoundListener.mActive)
{
}

SoundListener::~SoundListener()
{
}

/**
 * @brief Get active state.
 * @return Active state.
 */
bool SoundListener::GetActive() const
{
  return mActive;
}

/**
 * @brief Set active state.
 * @param aActive Active state.
 */
void SoundListener::SetActive(bool const &aActive)
{
  mActive = aActive;
}

/**
 * @brief Update loop
 */
void SoundListener::Update()
{
}

/**
 * @brief Receive message
 * @param aMessage Message to receive
 */
void SoundListener::ReceiveMessage(Message const& aMessage)
{
}

/**
 * @brief Send message
 * @param aMessage Message to send
 */
void SoundListener::SendMessage(Message const& aMessage)
{
}

/**
 * @brief Serialize to node
 * @param aNode Node to serialize to
 */
void SoundListener::Serialize(ParserNode* aNode)
{
  aNode->Place("SoundListener", "");
  
  ParserNode* node = aNode->Find("SoundListener");
  node->Place("Active", Common::BoolToString(mActive));
}

/**
 * @brief Deserialize from node
 * @param aNode Node to read from
 */
void SoundListener::Deserialize(ParserNode* aNode)
{
  if(aNode->Find("Active"))
  {
    mActive = aNode->Find("Active")->GetValue().ToBool();
  }
}

/**
 * @brief Clone SoundListener
 * @param aNewOwner The new owner
 * @return Cloned SoundListener
 */
Component* SoundListener::Clone(GameObject *aNewOwner) const
{
  return new SoundListener(*this);
}

/**
 * @brief Make this object usable in LUA
 */
void SoundListener::SerializeLUA()
{
  SLB::Class<SoundListener>("SoundListener")
    .inherits<Component>();
}

