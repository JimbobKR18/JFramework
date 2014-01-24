#ifndef __JFramework_MathExt_h_
#define __JFramework_MathExt_h_

#include <math.h>
#include "Common.h"

#define DEGREE_TO_RADS (3.141592f / 180.0f)
#define RADS_TO_DEGREE (180.0f / 3.141592f)

struct Vector3
{
  float x, y, z;

  Vector3();
  Vector3(float aX, float aY, float aZ);

  float   length() const;
  float   operator[](int aValue) const;
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
  Vector3 Project(Vector3 const &rhs) const;
  Vector3 Invert() const;

  void    operator+=(Vector3 const &rhs);
  void    operator-=(Vector3 const &rhs);
  void    operator*=(float const aMultiplier);
  void    operator/=(float const aMultiplier);

  static void SerializeLUA();
};

struct Vector4
{
  float x, y, z, w;

  Vector4();
  Vector4(float aX, float aY, float aZ, float aW);

  float   length() const;
  float   operator[](int aValue) const;
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

  static void SerializeLUA();
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

  Matrix33 operator*(Matrix33 const &rhs) const;
  Matrix33 operator*(float const aValue) const;
  Vector3  operator*(Vector3 const &rhs) const;

  void RotateX(float const aAngle);
  void RotateY(float const aAngle);
  void RotateZ(float const aAngle);
  void operator*=(Matrix33 const &rhs);
  void operator*=(float const aValue);
};

struct LineSegment
{
  Vector3 position;
  float length;

  LineSegment();
  LineSegment(Vector3 const &aPosition, float aLength);

  std::vector<Vector3> GetCollisions(LineSegment const &aCompare, int const aPrecision);
};

template<typename T>
class Interpolation
{
private:
  T*      mCurrent;
  T       mStart;
  T       mFinish;
  T       mRate;
  float   mTime;
  float   mCurrentTime;

public:
  Interpolation(T* aStart, T const& aFinish, float aTime) : mCurrent(aStart), mStart(*aStart), mFinish(aFinish), mTime(aTime), mCurrentTime(0)
  {
    mRate = mFinish - mStart;
  }
  virtual ~Interpolation() {}

  void Update(float aDT)
  {
    mCurrentTime += aDT;

    if(mCurrentTime < mTime)
    {
      float percentage = mCurrentTime / mTime;
      (*mCurrent) = mStart + (mRate * percentage);
    }
  }

  bool IsComplete()
  {
    return mCurrentTime >= mTime;
  }
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

#endif
