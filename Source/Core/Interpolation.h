#ifndef __JFramework_Interpolation_h_
#define __JFramework_Interpolation_h_

#include "LuaIncludes.h"

// Class to aid in Interpolation
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
  
  T GetCurrent() const
  {
    return *mCurrent;
  }
  
  T GetStart() const
  {
    return mStart;
  }
  
  T GetFinish() const
  {
    return mFinish;
  }
  
  T GetRate() const
  {
    return mRate;
  }

  bool IsComplete() const
  {
    return mCurrentTime >= mTime;
  }
  
  // Any datatype that is serialized in LUA must have the GetName() function defined.
  static void SerializeLUA()
  {
    HashString name = HashString("Interpolation_") + T::GetName();
    SLB::Class<Interpolation<T>>(name.ToCharArray())
      .set("Update", &Interpolation<T>::Update)
      .set("GetCurrent", &Interpolation<T>::GetCurrent)
      .set("GetStart", &Interpolation<T>::GetStart)
      .set("GetFinish", &Interpolation<T>::GetFinish)
      .set("GetRate", &Interpolation<T>::GetRate)
      .set("IsComplete", &Interpolation<T>::IsComplete);
  }
};

#endif