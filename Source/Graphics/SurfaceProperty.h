#ifndef __JFramework_SurfaceProperty_h_
#define __JFramework_SurfaceProperty_h_

#include "HashString.h"

enum PropertyType
{
  INT1,
  INT3,
  INT4,
  FLOAT1,
  FLOAT3,
  FLOAT4
};

class SurfaceProperty
{
private:
  HashString mName;
  PropertyType mType;
  HashString mTargetValue;
  HashString mDefaultValue;
  
public:
  SurfaceProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue);
  virtual ~SurfaceProperty();
  
  // Getters
  HashString GetName() const;
  PropertyType GetType() const;
  HashString GetTargetValue() const;
  HashString GetDefaultValue() const;
  
  // Setters
  void SetName(HashString const &aName);
  void SetType(PropertyType const &aType);
  void SetTargetValue(HashString const &aTargetValue);
  void SetDefaultValue(HashString const &aTargetValue);
};

#endif