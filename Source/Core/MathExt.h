#ifndef __JFramework_MathExt_h_
#define __JFramework_MathExt_h_

#include <cmath>
#include "Common.h"

#define PI 3.1415926f
#define DEGREE_TO_RADS (PI / 180.0f)
#define RADS_TO_DEGREE (180.0f / PI)
#define ROUNDING_ERROR 0.0001f

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

struct Vector2
{
  float x, y;
  
  Vector2();
  Vector2(float aX, float aY);
  
  float   length() const;
  float&  operator[](int const aValue);
  float   GetValue(int const aValue) const;
  Vector2 normalize() const;
  Vector2 abs() const;
  
  // Equivalence
  void    operator=(Vector2 const &rhs);
  bool    operator==(Vector2 const &rhs) const;
  
  // Scaling
  Vector2 operator*(float const aMultiplier) const;
  Vector2 operator/(float const aMultiplier) const;
  
  // Operator involving other vectors
  float   operator*(Vector2 const &rhs) const;
  Vector2 operator^(Vector2 const &rhs) const;
  Vector2 operator+(Vector2 const &rhs) const;
  Vector2 operator-(Vector2 const &rhs) const;
  Vector2 operator%(Vector2 const &rhs) const;
  Vector2 operator-() const;
  
  float   Dot(Vector2 const &rhs) const;
  Vector2 Cross(Vector2 const &rhs) const;
  Vector2 Add(Vector2 const &rhs) const;
  Vector2 Subtract(Vector2 const &rhs) const;
  Vector2 Multiply(Vector2 const &rhs) const;
  Vector2 Divide(Vector2 const &rhs) const;
  Vector2 Project(Vector2 const &rhs) const;
  Vector2 Invert() const;
  
  void    operator+=(Vector2 const &rhs);
  void    operator-=(Vector2 const &rhs);
  void    operator*=(Vector2 const &rhs);
  void    operator/=(Vector2 const &rhs);
  void    operator*=(float const aMultiplier);
  void    operator/=(float const aMultiplier);
  
  float   AngleBetweenRadians(Vector2 const &rhs) const;
  float   AngleBetweenDegrees(Vector2 const &rhs) const;
  
  std::vector<float> ToVector() const;
  HashString ToString() const;
  
  // Helpful debugging
  bool    validate() const;
  
  static void SerializeLUA();
  static HashString GetName();
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
  Vector3 abs() const;

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
  
  float   AngleBetweenRadians(Vector3 const &rhs, Vector3 const &aNormal) const;
  float   AngleBetweenDegrees(Vector3 const &rhs, Vector3 const &aNormal) const;
  
  std::vector<float> ToVector() const;
  HashString ToString() const;
  
  // Helpful debugging
  bool    validate() const;

  static void SerializeLUA();
  static HashString GetName();
};

struct Vector4
{
  float x, y, z, w;

  Vector4();
  Vector4(Vector3 const &aVector3);
  Vector4(float aX, float aY, float aZ, float aW);

  float   length() const;
  float   operator[](int aValue) const;
  float   GetValue(int const aValue) const;
  Vector4 normalize() const;
  Vector4 abs() const;

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
  
  float   AngleBetweenRadians(Vector4 const &rhs, Vector4 const &aNormal) const;
  float   AngleBetweenDegrees(Vector4 const &rhs, Vector4 const &aNormal) const;
  
  std::vector<float> ToVector() const;
  HashString ToString() const;
  
  // Helpful debugging
  bool validate() const;

  static void SerializeLUA();
  static HashString GetName();
};

struct Matrix33
{
  float values[3][3];

  Matrix33();
  Matrix33(float aValues[3][3]);
  Matrix33(float aValues[9]);
  Matrix33(Vector3 const &aValues);

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

  void Scale(Vector3 const &aScale);
  void RotateX(float const aAngle);
  void RotateY(float const aAngle);
  void RotateZ(float const aAngle);
  void operator*=(Matrix33 const &rhs);
  void operator*=(float const aValue);
  
  bool validate() const;
  
  static void SerializeLUA();
  static HashString GetName();
};

struct Matrix44
{
  float values[4][4];

  Matrix44();
  Matrix44(float aValues[4][4]);
  Matrix44(float aValues[16]);
  Matrix44(Vector4 const &aValues);

  Matrix44 Concatenate(Matrix44 const &rhs) const;
  Matrix44 Rotate(Vector4 const &aAxis, float const aAngle) const;
  Matrix44 Invert() const;
  float Determinant() const;
  float Determinant(float const aA, float const aB, float const aC, float const aD) const;
  
  void operator=(Matrix44 const &aMatrix);
  bool operator==(Matrix44 const &aMatrix) const;

  Matrix44 operator*(Matrix44 const &rhs) const;
  Matrix44 operator*(float const aValue) const;
  Vector4  operator*(Vector4 const &rhs) const;

  void Scale(Vector4 const &aScale);
  void RotateX(float const aAngle);
  void RotateY(float const aAngle);
  void RotateZ(float const aAngle);
  void RotateW(float const aAngle);
  void operator*=(Matrix44 const &rhs);
  void operator*=(float const aValue);
  
  bool validate() const;
  
  static void SerializeLUA();
  static HashString GetName();
};

struct Quaternion
{
  float x;
  float y;
  float z;
  float w;
  
  Quaternion normalize() const;

  // Constructors
  Quaternion();
  Quaternion(float aX, float aY, float aZ, float aW);
  Quaternion(float aX, float aY, float aZ);
  Quaternion(Vector3 const& aVec);

  // Operators
  bool operator==(Quaternion const& rhs) const;
  bool operator!=(Quaternion const& rhs) const;
  Quaternion& operator=(Quaternion const& rhs);
  Quaternion operator+(Quaternion const& rhs) const;
  Quaternion operator*(Quaternion const& rhs) const;
  Quaternion operator*(float aS) const;
  Quaternion& operator*=(float aS);
  Vector3 operator*(Vector3 const& aV) const;
  Quaternion& operator*=(Quaternion const& rhs);
  
  // Operations
  float Dot(Quaternion const& rhs) const;
  Quaternion& Set(float aX, float aY, float aZ, float aW);
  Quaternion& Set(float aX, float aY, float aZ);
  Quaternion& Set(Vector3 const& aVec);
  Quaternion& Set(Quaternion const& aQuat);
  bool Equals(Quaternion const& rhs, float const tolerance = ROUNDING_ERROR) const;
  Matrix44 GetMatrixFast() const;
  Matrix44 GetMatrix(Vector3 const& aTranslation) const;
  Matrix44 GetMatrixCenter(Vector3 const& aCenter, Vector3 const& aTranslation) const;
  Matrix44 GetMatrixTransposed() const;
  Quaternion& Invert();
  Quaternion& Lerp(Quaternion aQ1, Quaternion aQ2, float aTime);
  Quaternion& Slerp(Quaternion aQ1, Quaternion aQ2,
      float aTime, float aThreshold = ROUNDING_ERROR);
  Quaternion& FromAngleAxis(float aAngle, Vector3 const& aAxis);
  void ToAngleAxis(float &aAngle, Vector3& aAxis) const;
  Vector3 ToEuler() const;
  Quaternion& Identity();
  Quaternion& RotationFromTo(Vector3 const& aFrom, Vector3 const& aTo);
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

template<typename T>
T Clamp(T const &aRoot, T const &aLow, T const &aHigh)
{
  if(aRoot < aHigh)
    return (aRoot > aLow) ? aRoot : aLow;
  else
    return (aRoot < aHigh) ? aRoot : aHigh;
}

void print_matrix(Matrix33 const &_matrix);
void print_vector(Vector3 const &_point);

#endif
