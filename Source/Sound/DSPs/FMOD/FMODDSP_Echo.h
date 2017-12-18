#ifndef __JFramework__FMODDSP_Echo_h_
#define __JFramework__FMODDSP_Echo_h_

#include "DSP_Echo.h"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include "FMODSoundSystem.h"

class FMODDSP_Echo : public DSP_Echo
{
private:
  typedef std::unordered_map<int, FMOD::DSPConnection*>     FMODDSPConnectionContainer;
  typedef FMODDSPConnectionContainer::iterator              FMODDSPConnectionIt;

  FMOD::DSP* mDSP;
  FMODSoundSystem *mSoundSystem;
  FMODDSPConnectionContainer mConnectionContainer;
  
public:
  FMODDSP_Echo(FMOD::System* aSystem, FMODSoundSystem *aSoundSystem, HashString const &aName);
  virtual ~FMODDSP_Echo();
  
  FMOD::DSP* GetFMODDSP();
  
  virtual void SetFormat(int aNumChannels, Speaker_Mode const &aSpeakerMode);
  virtual void AddInput(DSP* aInput, float **aMixMatrix, int aWidth, int aHeight);
  virtual void RemoveInput(DSP* aInput);
  virtual void SetActive(bool const aActive);
  virtual void SetBypass(bool const aBypass);
  virtual void SetDelay(float aDelay);
  virtual void SetFeedback(float aFeedback);
  virtual void SetDryLevel(float aDryLevel);
  virtual void SetWetLevel(float aWetLevel);
};

#endif // __JFramework__FMODDSP_Echo_h_
