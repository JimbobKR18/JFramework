#ifndef __JFramework_RelativeLinearInterpolation_h_
#define __JFramework_RelativeLinearInterpolation_h_

#include "LuaIncludes.h"
#include "Interpolation.h"

// Class to aid in Interpolation
template<typename T>
class RelativeLinearInterpolation : public Interpolation<T>
{
private:
  T mStep;
  
public:
  RelativeLinearInterpolation(T* aStart, T const& aFinish, float aTime) : Interpolation<T>(aStart, aFinish, aTime)
  {
    mStep = (aFinish - *aStart) / aTime;
  }
  virtual ~RelativeLinearInterpolation() {}

  /**
   * @brief Updates interpolation, marks finished if complete.
   * @param aDT Change in millis
   */
  virtual void Update(float aDT)
  {
    Interpolation<T>::mCurrentTime += aDT;
    (*Interpolation<T>::mCurrent) += mStep * aDT;
  }
  
  /**
   * @brief Serialize to be usable in LUA.
   */
  static void SerializeLUA()
  {
    HashString name = HashString("RelativeLinearInterpolation_") + T::GetName();
    SLB::Class<RelativeLinearInterpolation<T>>(name.ToCharArray())
      .set("Update", &RelativeLinearInterpolation<T>::Update)
      .set("Complete", &RelativeLinearInterpolation<T>::Complete)
      .set("GetCurrent", &RelativeLinearInterpolation<T>::GetCurrent)
      .set("GetStart", &RelativeLinearInterpolation<T>::GetStart)
      .set("GetFinish", &RelativeLinearInterpolation<T>::GetFinish)
      .set("GetRate", &RelativeLinearInterpolation<T>::GetRate)
      .set("IsComplete", &RelativeLinearInterpolation<T>::IsComplete);
  }
};

#endif