#include "Framebuffer.h"

Framebuffer::Framebuffer() : mProperties(), mInputTextures(), mClearColor(0,0,0,1)
{
}

Framebuffer::~Framebuffer()
{
  ClearProperties();
  mInputTextures.clear();
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
    RenderableProperty *surfaceProperty = mProperties[aName.ToHash()];
    surfaceProperty->SetType(aType);
    surfaceProperty->SetTargetValue(aTargetValue);
    surfaceProperty->SetDefaultValue(aDefaultValue);
    return;
  }
  mProperties[aName.ToHash()] = new RenderableProperty(aName, aType, aTargetValue, aDefaultValue);
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

/**
 * @brief Set input textures
 * @return Input texture ids
 */
std::vector<int> const &Framebuffer::GetInputTextures() const
{
  return mInputTextures;
}

/**
 * @brief Get clear color
 * @return Clear color
 */
Vector4 const &Framebuffer::GetClearColor() const
{
  return mClearColor;
}

/**
 * @brief Set input textures.
 * @param aInputTextures Input texture ids.
 */
void Framebuffer::SetInputTextures(std::vector<int> const &aInputTextures)
{
  mInputTextures = aInputTextures;
}

/**
 * @brief Set clear color
 * @param aClearColor Clear color
 */
void Framebuffer::SetClearColor(Vector4 const &aClearColor)
{
  mClearColor = aClearColor;
}
