#ifndef __JFramework_SurfaceProperty_h_
#define __JFramework_SurfaceProperty_h_

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

class SurfaceProperty
{
private:
  HashString mName;
  PropertyType mType;
  HashString mTargetValue;
  HashString mDefaultValue;
  HashString mId;
  
public:
  SurfaceProperty(SurfaceProperty const &aSurfaceProperty);
  SurfaceProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue);
  SurfaceProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue, HashString const &aId);
  virtual ~SurfaceProperty();

  // Operations
  bool operator==(SurfaceProperty const &aRhs);
  bool operator!=(SurfaceProperty const &aRhs);
  
  // Getters
  HashString GetName() const;
  PropertyType GetType() const;
  HashString GetTargetValue() const;
  HashString GetDefaultValue() const;
  HashString GetId() const;
  
  // Setters
  void SetName(HashString const &aName);
  void SetType(PropertyType const &aType);
  void SetTargetValue(HashString const &aTargetValue);
  void SetDefaultValue(HashString const &aTargetValue);
  void SetId(HashString const &aId);
};

typedef std::unordered_map<int, SurfaceProperty*> PropertyContainer;
typedef PropertyContainer::iterator PropertyContainerIt;
typedef PropertyContainer::const_iterator PropertyContainerConstIt;

#endif
