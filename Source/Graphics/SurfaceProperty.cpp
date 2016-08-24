#include "Common.h"
#include "SurfaceProperty.h"

SurfaceProperty::SurfaceProperty(HashString const &aName, PropertyType const &aType, HashString const &aValue) : mName(aName), mType(aType), mValue(aValue) 
{
}

SurfaceProperty::~SurfaceProperty() 
{
}

/**
 * @brief Get name of property
 * @return Name
 */
HashString SurfaceProperty::GetName() const 
{
  return mName;
}

/**
 * @brief Get type of property
 * @return Type
 */
PropertyType SurfaceProperty::GetType() const
{
  return mType;
}

/**
 * @brief Get value of property
 * @return Value
 */
HashString SurfaceProperty::GetValue() const
{
  return mValue;
}

/**
 * @brief Set name of property
 * @param aName Name
 */
void SurfaceProperty::SetName(HashString const &aName)
{
  mName = aName;
}

/**
 * @brief Set type of property
 * @param aType Type
 */
void SurfaceProperty::SetType(PropertyType const &aType)
{
  mType = aType;
}

/**
 * @brief Set value of property
 * @param aValue Value
 */
void SurfaceProperty::SetValue(HashString const &aValue)
{
  mValue = aValue;
}