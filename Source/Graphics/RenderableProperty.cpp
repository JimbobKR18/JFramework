#include "Common.h"
#include "RenderableProperty.h"

RenderableProperty::RenderableProperty(RenderableProperty const &aRenderableProperty) :
  mName(aRenderableProperty.mName), mType(aRenderableProperty.mType), mTargetValue(aRenderableProperty.mTargetValue),
  mDefaultValue(aRenderableProperty.mDefaultValue), mId(aRenderableProperty.mId)
{
}

RenderableProperty::RenderableProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, 
  HashString const &aDefaultValue) : 
  mName(aName), mType(aType), mTargetValue(aTargetValue), mDefaultValue(aDefaultValue), mId("0")
{
}

RenderableProperty::RenderableProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, 
  HashString const &aDefaultValue, HashString const &aId) : 
  mName(aName), mType(aType), mTargetValue(aTargetValue), mDefaultValue(aDefaultValue), mId(aId)
{
}

RenderableProperty::~RenderableProperty() 
{
}

/**
 * @brief Equality check
 */
bool RenderableProperty::operator==(RenderableProperty const &aRhs)
{
  return mName == aRhs.mName && mTargetValue == aRhs.mTargetValue && mDefaultValue == aRhs.mDefaultValue &&
    mType == aRhs.mType && mId == aRhs.mId;
}

/**
* @brief Inequality check
*/
bool RenderableProperty::operator!=(RenderableProperty const &aRhs)
{
  return !(*this == aRhs);
}

/**
 * @brief Get name of property
 * @return Name
 */
HashString const& RenderableProperty::GetName() const 
{
  return mName;
}

/**
 * @brief Get type of property
 * @return Type
 */
PropertyType RenderableProperty::GetType() const
{
  return mType;
}

/**
 * @brief Get target value of property
 * @return Target value
 */
HashString const& RenderableProperty::GetTargetValue() const
{
  return mTargetValue;
}

/**
 * @brief Get default value of property
 * @return Default value
 */
HashString const& RenderableProperty::GetDefaultValue() const
{
  return mDefaultValue;
}

/**
 * @brief Get Id of property
 * @return Id
 */
HashString const& RenderableProperty::GetId() const
{
  return mId;
}

/**
 * @brief Set name of property
 * @param aName Name
 */
void RenderableProperty::SetName(HashString const &aName)
{
  mName = aName;
}

/**
 * @brief Set type of property
 * @param aType Type
 */
void RenderableProperty::SetType(PropertyType const &aType)
{
  mType = aType;
}

/**
 * @brief Set target value of property
 * @param aValue Target Value
 */
void RenderableProperty::SetTargetValue(HashString const &aTargetValue)
{
  mTargetValue = aTargetValue;
}

/**
 * @brief Set default value of property
 * @param aValue Default Value
 */
void RenderableProperty::SetDefaultValue(HashString const &aDefaultValue)
{
  mDefaultValue = aDefaultValue;
}

/**
 * @brief Set id
 * @param aId id
 */
void RenderableProperty::SetId(HashString const &aId)
{
  mId = aId;
}
