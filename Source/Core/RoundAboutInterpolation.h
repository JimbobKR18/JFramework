#ifndef __JFramework_RoundAboutInterpolation_h_
#define __JFramework_RoundAboutInterpolation_h_

#include "LuaIncludes.h"
#include "Interpolation.h"

// Class to aid in Interpolation
template<typename T>
class RoundAboutInterpolation : public Interpolation<T>
{
public:
  RoundAboutInterpolation(T* aStart, T const& aFinish, float aTime) : Interpolation<T>(aStart, aFinish, aTime)
  {
  }
  virtual ~RoundAboutInterpolation() {}

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
    float usablePercentage = percentage;
    if(percentage > 0.5f)
      usablePercentage = invertPercentage;
    
    (*Interpolation<T>::mCurrent) = mStart + (mRate * usablePercentage);
  }
  
  /**
   * @brief Serialize to be usable in LUA.
   */
  static void SerializeLUA()
  {
    HashString name = HashString("RoundAboutInterpolation_") + T::GetName();
    SLB::Class<RoundAboutInterpolation<T>>(name.ToCharArray())
      .set("Update", &RoundAboutInterpolation<T>::Update)
      .set("GetCurrent", &RoundAboutInterpolation<T>::GetCurrent)
      .set("GetStart", &RoundAboutInterpolation<T>::GetStart)
      .set("GetFinish", &RoundAboutInterpolation<T>::GetFinish)
      .set("GetRate", &RoundAboutInterpolation<T>::GetRate)
      .set("IsComplete", &RoundAboutInterpolation<T>::IsComplete);
  }
};

#endif