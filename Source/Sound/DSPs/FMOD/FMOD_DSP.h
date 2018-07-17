#ifndef __JFramework__FMOD_DSP_h_
#define __JFramework__FMOD_DSP_h_

#include "DSP.h"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include "FMODSoundSystem.h"

class FMOD_DSP : public DSP
{
private:
  typedef std::unordered_map<int, FMOD::DSPConnection*>     FMODDSPConnectionContainer;
  typedef FMODDSPConnectionContainer::iterator              FMODDSPConnectionIt;

  FMOD::DSP* mDSP;
  FMODSoundSystem *mSoundSystem;
  FMODDSPConnectionContainer mConnectionContainer;
  
public:
  FMOD_DSP(FMOD::DSP* aDSP, FMODSoundSystem *aSoundSystem, HashString const &aName);
  FMOD_DSP(FMOD::System* aSystem, FMODSoundSystem *aSoundSystem, HashString const &aName, int const &aType);
  virtual ~FMOD_DSP();
  
  FMOD::DSP* GetFMODDSP();

  virtual void SetFormat(int aNumChannels, Speaker_Mode const &aSpeakerMode);
  virtual void AddInput(DSP* aInput, float **aMixMatrix, int aWidth, int aHeight);
  virtual void RemoveInput(DSP* aInput);
  virtual void SetActive(bool const aActive);
  virtual void SetBypass(bool const aBypass);
  virtual void SetFloat(int const aParam, float const aValue);
  virtual void SetInt(int const aParam, int const aValue);
  virtual void SetBool(int const aParam, bool const aValue);
  virtual void SetData(int const aParam, void* aData, int const aLength);
  virtual void Deserialize(ParserNode *aNode);
};

#endif // __JFramework__FMOD_DSP_h_
