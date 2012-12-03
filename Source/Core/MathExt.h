#ifndef __JFramework_MathExt_h_
#define __JFramework_MathExt_h_

#include <math.h>

struct Vector3
{
  float x, y, z;

  Vector3();
  Vector3(float aX, float aY, float aZ);

  float length() const;
  Vector3 normalize() const;

  void operator=(Vector3 const &rhs);
  bool operator==(Vector3 const &rhs) const;

  Vector3 operator*(float const aMultiplier) const;
  Vector3 operator/(float const aMultiplier) const;

  float operator*(Vector3 const &rhs) const;
  Vector3 operator^(Vector3 const &rhs) const;
  Vector3 operator+(Vector3 const &rhs) const;
  Vector3 operator-(Vector3 const &rhs) const;
  Vector3 operator-() const;

  void operator+=(Vector3 const &rhs);
  void operator-=(Vector3 const &rhs);
  void operator*=(float const aMultiplier);
  void operator/=(float const aMultiplier);
};

#endif
