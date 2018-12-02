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
  SAMPLER2
};

class SurfaceProperty
{
private:
  HashString mName;
  PropertyType mType;
  HashString mTargetValue;
  HashString mDefaultValue;
  int mId;
  
public:
  SurfaceProperty(SurfaceProperty const &aSurfaceProperty);
  SurfaceProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue);
  SurfaceProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue, int const aId);
  virtual ~SurfaceProperty();
  
  // Getters
  HashString GetName() const;
  PropertyType GetType() const;
  HashString GetTargetValue() const;
  HashString GetDefaultValue() const;
  int GetId() const;
  
  // Setters
  void SetName(HashString const &aName);
  void SetType(PropertyType const &aType);
  void SetTargetValue(HashString const &aTargetValue);
  void SetDefaultValue(HashString const &aTargetValue);
  void SetId(int const aId);
};

typedef std::vector<SurfaceProperty*> PropertyContainer;
typedef PropertyContainer::iterator PropertyContainerIt;
typedef PropertyContainer::const_iterator PropertyContainerConstIt;

#endif
