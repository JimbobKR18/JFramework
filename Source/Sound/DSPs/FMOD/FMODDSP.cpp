#include "FMODDSP.h"

FMODDSP::FMODDSP(FMOD::System* aSystem, DSP_Type const &aType, HashString const &aName) : DSP(aName), mDSP(nullptr), mConnectionContainer()
{
  aSystem->createDSPByType((FMOD_DSP_TYPE)aType, &mDSP);
}

FMODDSP::FMODDSP(FMOD::DSP* aDSP, HashString const &aName) : DSP(aName), mDSP(aDSP), mConnectionContainer()
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

/**
 * @brief Set DSP formatting
 * @param aNumChannels
 * @param aSpeakerMode
 */
void FMODDSP::SetFormat(int aNumChannels, Speaker_Mode const &aSpeakerMode)
{
  mDSP->setChannelFormat(0, aNumChannels, (FMOD_SPEAKERMODE)aSpeakerMode);
}

/**
 * @brief Set DSP as input to this DSP
 * @param aDSP
 * @param aMixMatrix
 * @param aWidth
 * @param aHeight
 */
void FMODDSP::AddInput(DSP* aDSP, float **aMixMatrix, int aWidth, int aHeight)
{
  FMOD::DSPConnection *connection;
  FMODDSP *fmodDSP = (FMODDSP*)aDSP;
  mDSP->addInput(fmodDSP->GetFMODDSP(), &connection);
  connection->setMixMatrix(&aMixMatrix[0][0], aWidth, aHeight);
  mConnectionContainer[(GetName() + aDSP->GetName()).ToHash()] = connection;
}

/**
 * @brief Remove DSP as input
 * @param aInput
 */
void FMODDSP::RemoveInput(DSP* aInput)
{
  FMODDSP *fmodDSP = (FMODDSP*)aInput;
  mDSP->disconnectFrom(fmodDSP->GetFMODDSP(), mConnectionContainer[(GetName() + aInput->GetName()).ToHash()]);
}

/**
 * @brief Set DSP as active
 * @param aActive
 */
void FMODDSP::SetActive(bool const aActive)
{
  mDSP->setActive(aActive);
}

/**
 * @brief Set DSP bypass
 * @param aBypass
 */
void FMODDSP::SetBypass(bool const aBypass)
{
  mDSP->setBypass(aBypass);
}
