#include "FMODDSP.h"

FMODDSP::FMODDSP(FMOD::System* aSystem, DSP_Type const &aType, HashString const &aName) : DSP(aName), mDSP(nullptr)
{
  aSystem->createDSPByType((FMOD_DSP_TYPE)aType, &mDSP);
}

FMODDSP::FMODDSP(FMOD::DSP* aDSP, HashString const &aName) : DSP(aName), mDSP(aDSP)
{
}

FMODDSP::~FMODDSP()
{
  mDSP = nullptr;
}

FMOD::DSP* FMODDSP::GetFMODDSP()
{
  return mDSP;
}

void FMODDSP::SetActive(bool const aActive)
{
  mDSP->setActive(aActive);
}

void FMODDSP::SetBypass(bool const aBypass)
{
  mDSP->setBypass(aBypass);
}
