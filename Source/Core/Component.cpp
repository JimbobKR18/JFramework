#include "Component.h"
#include "LUA/LUATypes.h"

Component::Component()
{
  assert(!"Not supported.");
}

Component::Component(int const &aUID) : mOwner(nullptr), mUID(aUID) 
{
}

Component::Component(Component const &aComponent) : mOwner(aComponent.mOwner) 
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
int Component::GetDefinedUID() const
{
  return mUID;
}

/**
 * @brief Serialize component to LUA for use in scripts
 */
void Component::SerializeLUA()
{
  SLB::Class<Component, SLB::Instance::NoCopy>("Component")
    .set("GetOwner", &Component::GetOwner);
}
