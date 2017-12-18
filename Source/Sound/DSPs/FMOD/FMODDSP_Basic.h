#ifndef __JFramework__FMODDSP_Basic_h_
#define __JFramework__FMODDSP_Basic_h_

#include "FMODDSP.h"
#include "DSP.h"

class FMODDSP_Basic : public FMODDSP, public DSP
{
public:
  FMODDSP_Basic(FMOD::DSP* aDSP, HashString const &aName);
  virtual ~FMODDSP_Basic();

  virtual void SetFormat(int aNumChannels, Speaker_Mode const &aSpeakerMode);
  virtual void AddInput(DSP* aInput, float **aMixMatrix, int aWidth, int aHeight);
  virtual void RemoveInput(DSP* aInput);
  virtual void SetActive(bool const aActive);
  virtual void SetBypass(bool const aBypass);
};

#endif // __JFramework__FMODDSP_Basic_h_
