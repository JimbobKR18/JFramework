#include "MathExt.h"
#include <assert.h>

Vector3::Vector3() : x(0), y(0), z(0)
{
}
Vector3::Vector3(float aX, float aY, float aZ) : x(aX), y(aY), z(aZ)
{
}

float Vector3::length() const
{
  return sqrt((x*x) + (y*y) + (z*z));
}

float Vector3::operator[](int aValue) const
{
  switch(aValue)
  {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  default:
    assert(0);
  }
}

Vector3 Vector3::normalize() const
{
  Vector3 ret = *this;
  float const len = length();

  // Catch case if length is 0
  if(len == 0)
    return Vector3(1, 0, 0);

  ret.x /= len;
  ret.y /= len;
  ret.z /= len;

  return ret;
}

// Equivalence
void Vector3::operator=(Vector3 const &rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
}
bool Vector3::operator==(Vector3 const &rhs) const
{
  return ((int)x == (int)rhs.x && (int)y == (int)rhs.y && (int)z == (int)rhs.z);
}

// Scaling
Vector3 Vector3::operator*(float const aMultiplier) const
{
  Vector3 temp = *this;

  temp.x *= aMultiplier;
  temp.y *= aMultiplier;
  temp.z *= aMultiplier;

  return temp;
}
Vector3 Vector3::operator/(float const aMultiplier) const
{
  Vector3 temp = *this;

  temp.x /= aMultiplier;
  temp.y /= aMultiplier;
  temp.z /= aMultiplier;

  return temp;
}

// Operators with other vectors
float Vector3::operator*(Vector3 const &rhs) const
{
  return (x * rhs.x + y * rhs.y + z * rhs.z);
}
Vector3 Vector3::operator^(Vector3 const &rhs) const
{
  Vector3 temp = *this;

  temp.x = y * rhs.z - z * rhs.y;
  temp.y = z * rhs.x - x * rhs.z;
  temp.z = x * rhs.y - y * rhs.x;

  return temp;
}
Vector3 Vector3::operator+(Vector3 const &rhs) const
{
  Vector3 temp = *this;

  temp.x += rhs.x;
  temp.y += rhs.y;
  temp.z += rhs.z;

  return temp;
}
Vector3 Vector3::operator-(Vector3 const &rhs) const
{
  Vector3 temp = *this;

  temp.x -= rhs.x;
  temp.y -= rhs.y;
  temp.z -= rhs.z;

  return temp;
}
Vector3 Vector3::operator%(Vector3 const &rhs) const
{
  return rhs * ((*this * rhs)/(rhs * rhs));
}
Vector3 Vector3::operator-() const
{
  return (*this * -1.0f);
}

float Vector3::Dot(Vector3 const &rhs) const
{
  return *this * rhs;
}
Vector3 Vector3::Cross(Vector3 const &rhs) const
{
  return *this ^ rhs;
}
Vector3 Vector3::Add(Vector3 const &rhs) const
{
  return *this + rhs;
}
Vector3 Vector3::Subtract(Vector3 const &rhs) const
{
  return *this - rhs;
}
Vector3 Vector3::Project(Vector3 const &rhs) const
{
  return *this % rhs;
}
Vector3 Vector3::Invert() const
{
  return -(*this);
}

void Vector3::operator+=(Vector3 const &rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
}
void Vector3::operator-=(Vector3 const &rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
}
void Vector3::operator*=(float const aMultiplier)
{
  x *= aMultiplier;
  y *= aMultiplier;
  z *= aMultiplier;
}
void Vector3::operator/=(float const aMultiplier)
{
  x /= aMultiplier;
  y /= aMultiplier;
  z /= aMultiplier;
}
