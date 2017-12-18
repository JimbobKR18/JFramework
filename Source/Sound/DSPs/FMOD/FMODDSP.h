#ifndef __JFramework__FMODDSP_h_
#define __JFramework__FMODDSP_h_

#include "Common.h"
#include "DSP_Type.h"
#include "fmod_studio.hpp"
#include "fmod_errors.h"

class FMODDSP
{
protected:
  typedef std::unordered_map<int, FMOD::DSPConnection*>     FMODDSPConnectionContainer;
  typedef FMODDSPConnectionContainer::iterator              FMODDSPConnectionIt;

  FMOD::DSP* mDSP;
  FMODDSPConnectionContainer mConnectionContainer;

public:
  FMODDSP(FMOD::System* aSystem, DSP_Type const &aType);
  FMODDSP(FMOD::DSP* aDSP);
  virtual ~FMODDSP();
  
  FMOD::DSP* GetFMODDSP();
};

#endif // __JFramework__FMODDSP_h_
