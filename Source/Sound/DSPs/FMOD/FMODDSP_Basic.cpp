#include "FMODDSP_Basic.h"

FMODDSP_Basic::FMODDSP_Basic(FMOD::DSP* aDSP, HashString const &aName) : FMODDSP(aDSP), DSP(aName)
{
}

FMODDSP_Basic::~FMODDSP_Basic()
{
}

/**
 * @brief Set DSP formatting
 * @param aNumChannels
 * @param aSpeakerMode
 */
void FMODDSP_Basic::SetFormat(int aNumChannels, Speaker_Mode const &aSpeakerMode)
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
void FMODDSP_Basic::AddInput(DSP* aDSP, float **aMixMatrix, int aWidth, int aHeight)
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
void FMODDSP_Basic::RemoveInput(DSP* aInput)
{
  FMODDSP *fmodDSP = (FMODDSP*)aInput;
  mDSP->disconnectFrom(fmodDSP->GetFMODDSP(), mConnectionContainer[(GetName() + aInput->GetName()).ToHash()]);
}

/**
 * @brief Set DSP as active
 * @param aActive
 */
void FMODDSP_Basic::SetActive(bool const aActive)
{
  mDSP->setActive(aActive);
}

/**
 * @brief Set DSP bypass
 * @param aBypass
 */
void FMODDSP_Basic::SetBypass(bool const aBypass)
{
  mDSP->setBypass(aBypass);
}

