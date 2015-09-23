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

  /**
   * @brief Updates interpolation, marks finished if complete.
   * @param aDT Change in millis
   */
  void Update(float aDT)
  {
    mCurrentTime += aDT;

    if(mCurrentTime < mTime)
    {
      float percentage = mCurrentTime / mTime;
      (*mCurrent) = mStart + (mRate * percentage);
    }
  }
  
  /**
   * @brief Get value of current value in interpolation
   * @return Current value in interpolation
   */
  T GetCurrent() const
  {
    return *mCurrent;
  }
  
  /**
   * @brief Get start value.
   * @return Where our interpolation started.
   */
  T GetStart() const
  {
    return mStart;
  }
  
  /**
   * @brief Get end value.
   * @return Where the interpolation ends.
   */
  T GetFinish() const
  {
    return mFinish;
  }
  
  /**
   * @brief Get rate of change.
   * @return Rate of change.
   */
  T GetRate() const
  {
    return mRate;
  }

  /**
   * @brief Is our interpolation complete?
   * @return True if complete, false otherwise.
   */
  bool IsComplete() const
  {
    return mCurrentTime >= mTime;
  }
  
  /**
   * @brief Serialize to be usable in LUA.
   */
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