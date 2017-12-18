#ifndef __JFramework__DSP_Echo_h_
#define __JFramework__DSP_Echo_h_

#include "DSP.h"

class DSP_Echo : public DSP
{
public:
  DSP_Echo(HashString const& aName);
  virtual ~DSP_Echo();
  
  // Virtuals
  virtual void SetDelay(float aDelay) = 0;
  virtual void SetFeedback(float aFeedback) = 0;
  virtual void SetDryLevel(float aDryLevel) = 0;
  virtual void SetWetLevel(float aWetLevel) = 0;
};

#endif // __JFramework__DSP_Echo_h_
