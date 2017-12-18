#include "FMODDSP.h"

FMODDSP::FMODDSP(FMOD::System* aSystem, DSP_Type const &aType) : mDSP(nullptr), mConnectionContainer()
{
  aSystem->createDSPByType((FMOD_DSP_TYPE)aType, &mDSP);
}

FMODDSP::FMODDSP(FMOD::DSP* aDSP) : mDSP(aDSP), mConnectionContainer()
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
