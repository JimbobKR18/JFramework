#include "Framebuffer.h"

Framebuffer::Framebuffer() : mProperties()
{
}

Framebuffer::~Framebuffer()
{
  ClearProperties();
}

/**
 * @brief Get properties container
 * @return Container of properties
 */
PropertyContainer const& Framebuffer::GetProperties() const
{
  return mProperties;
}

/**
 * @brief Edit property if there, otherwise add.
 * @param aName Name of property
 * @param aType Type of property
 * @param aTargetValue Target value of property
 * @param aDefaultValue Default value of property
 */
void Framebuffer::AddOrEditProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue)
{
  if(mProperties.find(aName.ToHash()) != mProperties.end())
  {
    SurfaceProperty *surfaceProperty = mProperties[aName.ToHash()];
    surfaceProperty->SetType(aType);
    surfaceProperty->SetTargetValue(aTargetValue);
    surfaceProperty->SetDefaultValue(aDefaultValue);
    return;
  }
  mProperties[aName.ToHash()] = new SurfaceProperty(aName, aType, aTargetValue, aDefaultValue);
}

/**
 * @brief Clear properties for a surface.
 */
void Framebuffer::ClearProperties()
{
  PropertyContainerIt propertyEnd = mProperties.end();
  for(PropertyContainerIt it = mProperties.begin(); it != propertyEnd; ++it)
  {
    delete it->second;
  }
  mProperties.clear();
}
