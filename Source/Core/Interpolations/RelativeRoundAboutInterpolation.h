#ifndef __JFramework_RelativeRoundAboutInterpolation_h_
#define __JFramework_RelativeRoundAboutInterpolation_h_

#include "LuaIncludes.h"
#include "Interpolation.h"

// Class to aid in Interpolation
template<typename T>
class RelativeRoundAboutInterpolation : public Interpolation<T>
{
private:
  T mStep;
  
public:
  RelativeRoundAboutInterpolation(T* aStart, T const& aFinish, float aTime) : Interpolation<T>(aStart, aFinish, aTime)
  {
    mStep = (aFinish - *aStart) / aTime;
  }
  virtual ~RelativeRoundAboutInterpolation() {}

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
    T step = mStep;
    if(percentage > 0.5f)
      step = -mStep;
    
    (*Interpolation<T>::mCurrent) += step * aDT;
  }
  
  /**
   * @brief Serialize to be usable in LUA.
   */
  static void SerializeLUA()
  {
    HashString name = HashString("RelativeRoundAboutInterpolation_") + T::GetName();
    SLB::Class<RelativeRoundAboutInterpolation<T>>(name.ToCharArray())
      .set("Update", &RelativeRoundAboutInterpolation<T>::Update)
      .set("Complete", &RelativeRoundAboutInterpolation<T>::Complete)
      .set("GetCurrent", &RelativeRoundAboutInterpolation<T>::GetCurrent)
      .set("GetStart", &RelativeRoundAboutInterpolation<T>::GetStart)
      .set("GetFinish", &RelativeRoundAboutInterpolation<T>::GetFinish)
      .set("GetRate", &RelativeRoundAboutInterpolation<T>::GetRate)
      .set("IsComplete", &RelativeRoundAboutInterpolation<T>::IsComplete);
  }
};

#endif