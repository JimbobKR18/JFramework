#ifndef __JFramework_QuadraticInterpolation_h_
#define __JFramework_QuadraticInterpolation_h_

#include "LuaIncludes.h"
#include "Interpolation.h"

// Class to aid in Interpolation
template<typename T>
class QuadraticInterpolation : public Interpolation<T>
{
private:
  T mMidpoint;

public:
  QuadraticInterpolation(T* aStart, T const& aFinish, T const& aMidpoint, float aTime) : Interpolation<T>(aStart, aFinish, aTime), mMidpoint(aMidpoint)
  {
  }
  virtual ~QuadraticInterpolation() {}

  /**
   * @brief Updates interpolation, marks finished if complete.
   * @param aDT Change in millis
   */
  virtual void Update(float aDT)
  {
    Interpolation<T>::mCurrentTime += aDT;
    
    float timeNumerator = Interpolation<T>::mCurrentTime;
    if(timeNumerator > Interpolation<T>::mTime)
      timeNumerator = Interpolation<T>::mTime;
    
    float percentage = timeNumerator / Interpolation<T>::mTime;
    float invertPercentage = 1.0f - percentage;
    T firstHalf = ((Interpolation<T>::mStart * invertPercentage) + (mMidpoint * percentage)) * invertPercentage;
    T secondHalf = ((mMidpoint * invertPercentage) + (Interpolation<T>::mFinish * percentage)) * percentage;
    
    (*Interpolation<T>::mCurrent) = firstHalf + secondHalf;
  }
  
  /**
   * @brief Serialize to be usable in LUA.
   */
  static void SerializeLUA()
  {
    HashString name = HashString("QuadraticInterpolation_") + T::GetName();
    SLB::Class<QuadraticInterpolation<T>>(name.ToCharArray())
      .set("Update", &QuadraticInterpolation<T>::Update)
      .set("Complete", &QuadraticInterpolation<T>::Complete)
      .set("GetCurrent", &QuadraticInterpolation<T>::GetCurrent)
      .set("GetStart", &QuadraticInterpolation<T>::GetStart)
      .set("GetFinish", &QuadraticInterpolation<T>::GetFinish)
      .set("GetRate", &QuadraticInterpolation<T>::GetRate)
      .set("IsComplete", &QuadraticInterpolation<T>::IsComplete);
  }
};

#endif