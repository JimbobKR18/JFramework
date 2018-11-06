#include "SoundListener.h"
#include "LUATypes.h"

int const SoundListener::sUID = Common::StringHashFunction("SoundListener");

SoundListener::SoundListener() : Component(SoundListener::sUID)
{
}

SoundListener::SoundListener(SoundListener const &aSoundListener) : Component(SoundListener::sUID)
{
}

SoundListener::~SoundListener()
{
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
}

/**
 * @brief Deserialize from node
 * @param aNode Node to read from
 */
void SoundListener::Deserialize(ParserNode* aNode)
{
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

