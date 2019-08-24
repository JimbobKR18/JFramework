#include "Component.h"
#include "LUA/LUATypes.h"

Component::Component()
{
  assert(!"Not supported.");
}

Component::Component(HashType const &aUID) : mOwner(nullptr), mInitialized(false), mUID(aUID) 
{
}

Component::Component(Component const &aComponent) : mOwner(aComponent.mOwner), mInitialized(aComponent.mInitialized)
{
}

Component::~Component() 
{
  mOwner = nullptr;
}

/**
 * @brief Get owner of component
 * @return Owner
 */
GameObject* Component::GetOwner() const 
{
  return mOwner;
}

/**
 * @brief Set owner of component
 * @param aOwner New owner
 */
void Component::SetOwner(GameObject *aOwner) 
{
  if(mOwner && aOwner != nullptr) 
    assert(!"This component already has an owner."); 
  mOwner = aOwner;
}

/**
 * @brief Get UID of component
 * @return UID
 */
HashType Component::GetDefinedUID() const
{
  return mUID;
}

/**
 * @brief Get initialization state of component
 * @return True if initialized
 */
bool Component::GetInitialized() const
{
  return mInitialized;
}

/**
 * @brief Override this function to have a one time call on first update
 */
void Component::Initialize()
{
  mInitialized = true;
}

/**
 * @brief Serialize component to LUA for use in scripts
 */
void Component::SerializeLUA()
{
  SLB::Class<Component, SLB::Instance::NoCopy>("Component")
    .set("GetOwner", &Component::GetOwner);
}
