#ifndef __JFramework__FMODDSP_Basic_h_
#define __JFramework__FMODDSP_Basic_h_

#include "DSP.h"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include "FMODSoundSystem.h"

class FMODDSP_Basic : public DSP
{
private:
  typedef std::unordered_map<int, FMOD::DSPConnection*>     FMODDSPConnectionContainer;
  typedef FMODDSPConnectionContainer::iterator              FMODDSPConnectionIt;

  FMOD::DSP* mDSP;
  FMODSoundSystem *mSoundSystem;
  FMODDSPConnectionContainer mConnectionContainer;
  
public:
  FMODDSP_Basic(FMOD::DSP* aDSP, FMODSoundSystem *aSoundSystem, HashString const &aName);
  virtual ~FMODDSP_Basic();
  
  FMOD::DSP* GetFMODDSP();

  virtual void SetFormat(int aNumChannels, Speaker_Mode const &aSpeakerMode);
  virtual void AddInput(DSP* aInput, float **aMixMatrix, int aWidth, int aHeight);
  virtual void RemoveInput(DSP* aInput);
  virtual void SetActive(bool const aActive);
  virtual void SetBypass(bool const aBypass);
};

#endif // __JFramework__FMODDSP_Basic_h_
