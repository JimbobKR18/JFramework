#include "Common.h"
#include "SurfaceProperty.h"

SurfaceProperty::SurfaceProperty(SurfaceProperty const &aSurfaceProperty) :
  mName(aSurfaceProperty.mName), mType(aSurfaceProperty.mType), mTargetValue(aSurfaceProperty.mTargetValue),
  mDefaultValue(aSurfaceProperty.mDefaultValue), mId(aSurfaceProperty.mId)
{
}

SurfaceProperty::SurfaceProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, 
  HashString const &aDefaultValue) : 
  mName(aName), mType(aType), mTargetValue(aTargetValue), mDefaultValue(aDefaultValue), mId("0")
{
}

SurfaceProperty::SurfaceProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, 
  HashString const &aDefaultValue, HashString const &aId) : 
  mName(aName), mType(aType), mTargetValue(aTargetValue), mDefaultValue(aDefaultValue), mId(aId)
{
}

SurfaceProperty::~SurfaceProperty() 
{
}

/**
 * @brief Equality check
 */
bool SurfaceProperty::operator==(SurfaceProperty const &aRhs)
{
  return mName == aRhs.mName && mTargetValue == aRhs.mTargetValue && mDefaultValue == aRhs.mDefaultValue &&
    mType == aRhs.mType && mId == aRhs.mId;
}

/**
* @brief Inequality check
*/
bool SurfaceProperty::operator!=(SurfaceProperty const &aRhs)
{
  return !(*this == aRhs);
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
 * @brief Get Id of property
 * @return Id
 */
HashString SurfaceProperty::GetId() const
{
  return mId;
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

/**
 * @brief Set id
 * @param aId id
 */
void SurfaceProperty::SetId(HashString const &aId)
{
  mId = aId;
}
