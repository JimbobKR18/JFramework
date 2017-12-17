#ifndef __JFramework__DSP_h_
#define __JFramework__DSP_h_

#include "Common.h"
#include "DSP_Type.h"

class DSP
{
private:
  HashString mName;
  
public:
  DSP(HashString const& aName);
  virtual ~DSP();
  
  HashString const GetName() const;
  
  // Virtuals
  virtual void SetFormat(int aNumChannels, Speaker_Mode const &aSpeakerMode) = 0;
  virtual void AddInput(DSP* aInput, float **aMixMatrix, int aWidth, int aHeight) = 0;
  virtual void RemoveInput(DSP* aInput) = 0;
  virtual void SetActive(bool const aActive) = 0;
  virtual void SetBypass(bool const aBypass) = 0;
};

#endif // __JFramework__DSP_h_
