#include "MathExt.h"
#include "LuaIncludes.h"
#include <assert.h>

//--------------------------------
// VECTOR3
//--------------------------------
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

void Vector3::SerializeLUA()
{
  SLB::Class<Vector3>("Vector3").constructor<float,float,float>()
          .set("x", &Vector3::x)
          .set("y", &Vector3::y)
          .set("z", &Vector3::z)
          .set("length", &Vector3::length)
          .set("SetEqual", &Vector3::operator=)
          .set("IsEqual", &Vector3::operator==)
          .set("Add", &Vector3::Add)
          .set("Subtract", &Vector3::Subtract)
          .set("Dot", &Vector3::Dot)
          .set("Cross", &Vector3::Cross)
          .set("Project", &Vector3::Project)
          .set("Invert", &Vector3::Invert);
}

//------------------------------
// VECTOR4
//------------------------------
Vector4::Vector4() : x(0), y(0), z(0), w(0)
{
}
Vector4::Vector4(float aX, float aY, float aZ, float aW) : x(aX), y(aY), z(aZ), w(aW)
{
}

float Vector4::length() const
{
  return sqrt((x*x) + (y*y) + (z*z) + (w*w));
}

float Vector4::operator[](int aValue) const
{
  switch(aValue)
  {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  case 3:
    return w;
  default:
    assert(0);
  }
}

Vector4 Vector4::normalize() const
{
  Vector4 ret = *this;
  float const len = length();

  // Catch case if length is 0
  if(len == 0)
    return Vector4(1, 0, 0, 0);

  ret.x /= len;
  ret.y /= len;
  ret.z /= len;
  ret.w /= len;

  return ret;
}

// Equivalence
void Vector4::operator=(Vector4 const &rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  w = rhs.w;
}
bool Vector4::operator==(Vector4 const &rhs) const
{
  return ((int)x == (int)rhs.x && (int)y == (int)rhs.y && (int)z == (int)rhs.z && (int)w == (int)rhs.w);
}

// Scaling
Vector4 Vector4::operator*(float const aMultiplier) const
{
  Vector4 temp = *this;

  temp.x *= aMultiplier;
  temp.y *= aMultiplier;
  temp.z *= aMultiplier;
  temp.w *= aMultiplier;

  return temp;
}
Vector4 Vector4::operator/(float const aMultiplier) const
{
  Vector4 temp = *this;

  temp.x /= aMultiplier;
  temp.y /= aMultiplier;
  temp.z /= aMultiplier;
  temp.w /= aMultiplier;

  return temp;
}

// Operators with other vectors
float Vector4::operator*(Vector4 const &rhs) const
{
  return (x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w);
}
Vector3 Vector4::operator^(Vector4 const &rhs) const
{
  Vector3 temp = Vector3(x, y, z);

  temp.x = y * rhs.z - z * rhs.y;
  temp.y = z * rhs.x - x * rhs.z;
  temp.z = x * rhs.y - y * rhs.x;

  return temp;
}
Vector4 Vector4::operator+(Vector4 const &rhs) const
{
  Vector4 temp = *this;

  temp.x += rhs.x;
  temp.y += rhs.y;
  temp.z += rhs.z;
  temp.w += rhs.w;

  return temp;
}
Vector4 Vector4::operator-(Vector4 const &rhs) const
{
  Vector4 temp = *this;

  temp.x -= rhs.x;
  temp.y -= rhs.y;
  temp.z -= rhs.z;
  temp.w -= rhs.w;

  return temp;
}
Vector4 Vector4::operator%(Vector4 const &rhs) const
{
  return rhs * ((*this * rhs)/(rhs * rhs));
}
Vector4 Vector4::operator-() const
{
  return (*this * -1.0f);
}

float Vector4::Dot(Vector4 const &rhs) const
{
  return *this * rhs;
}
Vector3 Vector4::Cross(Vector4 const &rhs) const
{
  return *this ^ rhs;
}
Vector4 Vector4::Add(Vector4 const &rhs) const
{
  return *this + rhs;
}
Vector4 Vector4::Subtract(Vector4 const &rhs) const
{
  return *this - rhs;
}
Vector4 Vector4::Project(Vector4 const &rhs) const
{
  return *this % rhs;
}
Vector4 Vector4::Invert() const
{
  return -(*this);
}

void Vector4::operator+=(Vector4 const &rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  w += rhs.w;
}
void Vector4::operator-=(Vector4 const &rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  w -= rhs.w;
}
void Vector4::operator*=(float const aMultiplier)
{
  x *= aMultiplier;
  y *= aMultiplier;
  z *= aMultiplier;
  w *= aMultiplier;
}
void Vector4::operator/=(float const aMultiplier)
{
  x /= aMultiplier;
  y /= aMultiplier;
  z /= aMultiplier;
  w /= aMultiplier;
}

void Vector4::SerializeLUA()
{
  SLB::Class<Vector4>("Vector4").constructor<float,float,float,float>()
          .set("x", &Vector4::x)
          .set("y", &Vector4::y)
          .set("z", &Vector4::z)
          .set("w", &Vector4::w)
          .set("length", &Vector4::length)
          .set("SetEqual", &Vector4::operator=)
          .set("IsEqual", &Vector4::operator==)
          .set("Add", &Vector4::Add)
          .set("Subtract", &Vector4::Subtract)
          .set("Dot", &Vector4::Dot)
          .set("Cross", &Vector4::Cross)
          .set("Project", &Vector4::Project)
          .set("Invert", &Vector4::Invert);
}
