#ifndef __JFramework__DSP_FMOD_h_
#define __JFramework__DSP_FMOD_h_

#include "DSP.h"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include "FMODSoundSystem.h"

class DSP_FMOD : public DSP
{
private:
  typedef std::unordered_map<HashType, FMOD::DSPConnection*>     FMODDSPConnectionContainer;
  typedef FMODDSPConnectionContainer::iterator              FMODDSPConnectionIt;

  FMOD::DSP* mDSP;
  FMODSoundSystem *mSoundSystem;
  FMODDSPConnectionContainer mConnectionContainer;
  
public:
  DSP_FMOD(FMOD::DSP* aDSP, FMODSoundSystem *aSoundSystem, HashString const &aName);
  DSP_FMOD(FMOD::System* aSystem, FMODSoundSystem *aSoundSystem, HashString const &aName, int const &aType);
  virtual ~DSP_FMOD();
  
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

#endif // __JFramework__DSP_FMOD_h_
