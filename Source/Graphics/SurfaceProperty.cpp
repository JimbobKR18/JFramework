#include "Common.h"
#include "SurfaceProperty.h"

SurfaceProperty::SurfaceProperty(SurfaceProperty const &aSurfaceProperty) :
  mName(aSurfaceProperty.mName), mType(aSurfaceProperty.mType), mTargetValue(aSurfaceProperty.mTargetValue),
  mDefaultValue(aSurfaceProperty.mDefaultValue)
{
}

SurfaceProperty::SurfaceProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue) : 
  mName(aName), mType(aType), mTargetValue(aTargetValue), mDefaultValue(aDefaultValue) 
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
 * @brief Get target value of property
 * @return Target value
 */
HashString SurfaceProperty::GetTargetValue() const
{
  return mTargetValue;
}

/**
 * @brief Get default value of property
 * @return Default value
 */
HashString SurfaceProperty::GetDefaultValue() const
{
  return mDefaultValue;
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
 * @brief Set target value of property
 * @param aValue Target Value
 */
void SurfaceProperty::SetTargetValue(HashString const &aTargetValue)
{
  mTargetValue = aTargetValue;
}

/**
 * @brief Set default value of property
 * @param aValue Default Value
 */
void SurfaceProperty::SetDefaultValue(HashString const &aDefaultValue)
{
  mDefaultValue = aDefaultValue;
}
