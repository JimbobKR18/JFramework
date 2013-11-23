#ifndef __JFramework_MathExt_h_
#define __JFramework_MathExt_h_

#include <math.h>

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

  void Update(float dt)
  {
    mCurrentTime += dt;

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

#endif
