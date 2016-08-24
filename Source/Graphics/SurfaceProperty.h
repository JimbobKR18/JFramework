#ifndef __JFramework_SurfaceProperty_h_
#define __JFramework_SurfaceProperty_h_

#include "HashString.h"

enum PropertyType
{
  INT1,
  INT3,
  FLOAT1,
  FLOAT3
};

class SurfaceProperty
{
private:
  HashString mName;
  PropertyType mType;
  HashString mValue;
  
public:
  SurfaceProperty(HashString const &aName, PropertyType const &aType, HashString const &aValue);
  virtual ~SurfaceProperty();
  
  // Getters
  HashString GetName() const;
  PropertyType GetType() const;
  HashString GetValue() const;
  
  // Setters
  void SetName(HashString const &aName);
  void SetType(PropertyType const &aType);
  void SetValue(HashString const &aValue);
};

#endif