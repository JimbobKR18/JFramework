#ifndef __JFramework_RenderableProperty_h_
#define __JFramework_RenderableProperty_h_

#include "HashString.h"

enum PropertyType
{
  INT1,
  INT2,
  INT3,
  INT4,
  FLOAT1,
  FLOAT2,
  FLOAT3,
  FLOAT4,
  SAMPLER2,
  INT1VECTOR,
  INT2VECTOR,
  INT3VECTOR,
  INT4VECTOR,
  FLOAT1VECTOR,
  FLOAT2VECTOR,
  FLOAT3VECTOR,
  FLOAT4VECTOR,
  SAMPLER2VECTOR
};

class RenderableProperty
{
private:
  HashString mName;
  PropertyType mType;
  HashString mTargetValue;
  HashString mDefaultValue;
  HashString mId;
  
public:
  RenderableProperty(RenderableProperty const &aRenderableProperty);
  RenderableProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue);
  RenderableProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue, HashString const &aId);
  virtual ~RenderableProperty();

  // Operations
  bool operator==(RenderableProperty const &aRhs);
  bool operator!=(RenderableProperty const &aRhs);
  
  // Getters
  HashString const& GetName() const;
  PropertyType GetType() const;
  HashString const& GetTargetValue() const;
  HashString const& GetDefaultValue() const;
  HashString const& GetId() const;
  
  // Setters
  void SetName(HashString const &aName);
  void SetType(PropertyType const &aType);
  void SetTargetValue(HashString const &aTargetValue);
  void SetDefaultValue(HashString const &aTargetValue);
  void SetId(HashString const &aId);
};

typedef std::unordered_map<HashType, RenderableProperty*> PropertyContainer;
typedef PropertyContainer::iterator PropertyContainerIt;
typedef PropertyContainer::const_iterator PropertyContainerConstIt;

#endif
