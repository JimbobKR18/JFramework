#ifndef __JFramework__FMODDSP_Echo_h_
#define __JFramework__FMODDSP_Echo_h_

#include "FMODDSP.h"
#include "DSP_Echo.h"

class FMODDSP_Echo : public FMODDSP, DSP_Echo
{
public:
  FMODDSP_Echo(FMOD::System* aSystem, HashString const &aName);
  virtual ~FMODDSP_Echo();
  
  virtual void SetDelay(float aDelay);
  virtual void SetFeedback(float aFeedback);
  virtual void SetDryLevel(float aDryLevel);
  virtual void SetWetLevel(float aWetLevel);
};

#endif // __JFramework__FMODDSP_Echo_h_
