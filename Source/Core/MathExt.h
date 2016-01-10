#ifndef __JFramework_MathExt_h_
#define __JFramework_MathExt_h_

#include <math.h>
#include "Common.h"

#define DEGREE_TO_RADS (3.141592f / 180.0f)
#define RADS_TO_DEGREE (180.0f / 3.141592f)

enum X_ALIGNMENT
{
  X_ALIGN_LEFT = 0,
  X_ALIGN_CENTER,
  X_ALIGN_RIGHT
};

enum Y_ALIGNMENT
{
  Y_ALIGN_TOP = 0,
  Y_ALIGN_CENTER,
  Y_ALIGN_BOTTOM
};

enum Z_ALIGNMENT
{
  Z_ALIGN_FRONT = 0,
  Z_ALIGN_CENTER,
  Z_ALIGN_BACK
};

struct Vector3
{
  float x, y, z;

  Vector3();
  Vector3(float aX, float aY, float aZ);

  float   length() const;
  float&  operator[](int const aValue);
  float   GetValue(int const aValue) const;
  Vector3 normalize() const;

  // Equivalence
  void    operator=(Vector3 const &rhs);
  bool    operator==(Vector3 const &rhs) const;

  // Scaling
  Vector3 operator*(float const aMultiplier) const;
  Vector3 operator/(float const aMultiplier) const;

  // Operator involving other vectors
  float   operator*(Vector3 const &rhs) const;
  Vector3 operator^(Vector3 const &rhs) const;
  Vector3 operator+(Vector3 const &rhs) const;
  Vector3 operator-(Vector3 const &rhs) const;
  Vector3 operator%(Vector3 const &rhs) const;
  Vector3 operator-() const;

  float   Dot(Vector3 const &rhs) const;
  Vector3 Cross(Vector3 const &rhs) const;
  Vector3 Add(Vector3 const &rhs) const;
  Vector3 Subtract(Vector3 const &rhs) const;
  Vector3 Multiply(Vector3 const &rhs) const;
  Vector3 Divide(Vector3 const &rhs) const;
  Vector3 Project(Vector3 const &rhs) const;
  Vector3 Invert() const;

  void    operator+=(Vector3 const &rhs);
  void    operator-=(Vector3 const &rhs);
  void    operator*=(Vector3 const &rhs);
  void    operator/=(Vector3 const &rhs);
  void    operator*=(float const aMultiplier);
  void    operator/=(float const aMultiplier);
  
  // Helpful debugging
  bool    validate() const;

  static void SerializeLUA();
  static std::string GetName();
};

struct Vector4
{
  float x, y, z, w;

  Vector4();
  Vector4(float aX, float aY, float aZ, float aW);

  float   length() const;
  float   operator[](int aValue) const;
  float   GetValue(int const aValue) const;
  Vector4 normalize() const;

  // Equivalence
  void    operator=(Vector4 const &rhs);
  bool    operator==(Vector4 const &rhs) const;

  // Scaling
  Vector4 operator*(float const aMultiplier) const;
  Vector4 operator/(float const aMultiplier) const;

  // Operator involving other vectors
  float   operator*(Vector4 const &rhs) const;
  Vector3 operator^(Vector4 const &rhs) const;
  Vector4 operator+(Vector4 const &rhs) const;
  Vector4 operator-(Vector4 const &rhs) const;
  Vector4 operator%(Vector4 const &rhs) const;
  Vector4 operator-() const;

  float   Dot(Vector4 const &rhs) const;
  Vector3 Cross(Vector4 const &rhs) const;
  Vector4 Add(Vector4 const &rhs) const;
  Vector4 Subtract(Vector4 const &rhs) const;
  Vector4 Project(Vector4 const &rhs) const;
  Vector4 Invert() const;

  void    operator+=(Vector4 const &rhs);
  void    operator-=(Vector4 const &rhs);
  void    operator*=(float const aMultiplier);
  void    operator/=(float const aMultiplier);
  
  // Helpful debugging
  bool validate() const;

  static void SerializeLUA();
  static std::string GetName();
};

struct Matrix33
{
  float values[3][3];

  Matrix33();
  Matrix33(float aValues[3][3]);
  Matrix33(float aValues[9]);

  Matrix33 Concatenate(Matrix33 const &rhs) const;
  Matrix33 Rotate(Vector3 const &aAxis, float const aAngle) const;
  Matrix33 Invert() const;
  float Determinant() const;
  float Determinant(float const aA, float const aB, float const aC, float const aD) const;
  
  void operator=(Matrix33 const &aMatrix);
  bool operator==(Matrix33 const &aMatrix) const;

  Matrix33 operator*(Matrix33 const &rhs) const;
  Matrix33 operator*(float const aValue) const;
  Vector3  operator*(Vector3 const &rhs) const;

  void RotateX(float const aAngle);
  void RotateY(float const aAngle);
  void RotateZ(float const aAngle);
  void operator*=(Matrix33 const &rhs);
  void operator*=(float const aValue);
  
  bool validate() const;
  
  static void SerializeLUA();
  static std::string GetName();
};

template<typename T>
T Lesser(T const& aA, T const& aB)
{
  if(aA <= aB)
    return aA;
  return aB;
}

template<typename T>
T Greater(T const& aA, T const& aB)
{
  if(aA >= aB)
    return aA;
  return aB;
}

template<typename T>
bool WithinRange(T const& aRoot, T const& aCompare, T const& aRange)
{
  T max = aRoot + aRange;
  T min = aRoot - aRange;
  if(max > aCompare && min < aCompare)
  {
    return true;
  }
  return false;
}

void print_matrix(Matrix33 const &_matrix);
void print_vector(Vector3 const &_point);

#endif
