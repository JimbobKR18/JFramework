#ifndef __JFramework__FMODDSP_h_
#define __JFramework__FMODDSP_h_

#include "DSP.h"
#include "DSP_Type.h"
#include "fmod_studio.hpp"
#include "fmod_errors.h"

class FMODDSP : public DSP
{
private:
  FMOD::DSP* mDSP;

public:
  FMODDSP(FMOD::System* aSystem, DSP_Type const &aType, HashString const &aName);
  FMODDSP(FMOD::DSP* aDSP, HashString const &aName);
  virtual ~FMODDSP();
  
  FMOD::DSP* GetFMODDSP();
  
  virtual void SetActive(bool const aActive);
  virtual void SetBypass(bool const aBypass);
};

#endif // __JFramework__FMODDSP_h_
