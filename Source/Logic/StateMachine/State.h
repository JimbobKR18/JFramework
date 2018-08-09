#ifndef __JFRAMEWORK_STATE_H
#define __JFRAMEWORK_STATE_H

#include "HashString.h"

#define INFINITE_TIME_ALIVE -1.0f

class State
{
private:
  HashString mName;
  float mTimeAlive;
  
  State();
public:
  State(HashString const &aName);
  State(HashString const &aName, float const &aTimeAlive);
  virtual ~State();
  
  HashString GetName() const;
  float GetTimeAlive() const;
  void SetName(HashString const &aName);
  void SetTimeAlive(float const &aTimeAlive);
  
  static void SerializeLUA();
};

#endif // __JFRAMEWORK_STATE_H
