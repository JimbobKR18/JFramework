#ifndef __JFramework__FMODDSP_h_
#define __JFramework__FMODDSP_h_

#include "DSP.h"
#include "DSP_Type.h"
#include "fmod_studio.hpp"
#include "fmod_errors.h"

class FMODDSP : public DSP
{
private:
  typedef std::unordered_map<int, FMOD::DSPConnection*>     FMODDSPConnectionContainer;
  typedef FMODDSPConnectionContainer::iterator              FMODDSPConnectionIt;

  FMOD::DSP* mDSP;
  FMODDSPConnectionContainer mConnectionContainer;

public:
  FMODDSP(FMOD::System* aSystem, DSP_Type const &aType, HashString const &aName);
  FMODDSP(FMOD::DSP* aDSP, HashString const &aName);
  virtual ~FMODDSP();
  
  FMOD::DSP* GetFMODDSP();
  
  virtual void SetFormat(int aNumChannels, Speaker_Mode const &aSpeakerMode);
  virtual void AddInput(DSP* aInput, float **aMixMatrix, int aWidth, int aHeight);
  virtual void RemoveInput(DSP* aInput);
  virtual void SetActive(bool const aActive);
  virtual void SetBypass(bool const aBypass);
};

#endif // __JFramework__FMODDSP_h_
