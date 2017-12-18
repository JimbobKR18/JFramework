#include "FMODDSP_Echo.h"

FMODDSP_Echo::FMODDSP_Echo(FMOD::System* aSystem, HashString const &aName) : FMODDSP(aSystem, DSP_TYPE_ECHO), DSP_Echo(aName)
{
}

FMODDSP_Echo::~FMODDSP_Echo()
{
}

/**
 * @brief Set DSP formatting
 * @param aNumChannels
 * @param aSpeakerMode
 */
void FMODDSP_Echo::SetFormat(int aNumChannels, Speaker_Mode const &aSpeakerMode)
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
void FMODDSP_Echo::AddInput(DSP* aDSP, float **aMixMatrix, int aWidth, int aHeight)
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
void FMODDSP_Echo::RemoveInput(DSP* aInput)
{
  FMODDSP *fmodDSP = (FMODDSP*)aInput;
  mDSP->disconnectFrom(fmodDSP->GetFMODDSP(), mConnectionContainer[(GetName() + aInput->GetName()).ToHash()]);
}

/**
 * @brief Set DSP as active
 * @param aActive
 */
void FMODDSP_Echo::SetActive(bool const aActive)
{
  mDSP->setActive(aActive);
}

/**
 * @brief Set DSP bypass
 * @param aBypass
 */
void FMODDSP_Echo::SetBypass(bool const aBypass)
{
  mDSP->setBypass(aBypass);
}

/**
 * @brief Set delay
 * @param aDelay
 */
void FMODDSP_Echo::SetDelay(float aDelay)
{
  GetFMODDSP()->setParameterFloat(FMOD_DSP_ECHO_DELAY, aDelay);
}

/**
 * @brief Set feedback
 * @param aFeedback
 */
void FMODDSP_Echo::SetFeedback(float aFeedback)
{
  GetFMODDSP()->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, aFeedback);
}

/**
 * @brief Set dry level
 * @param aDryLevel
 */
void FMODDSP_Echo::SetDryLevel(float aDryLevel)
{
  GetFMODDSP()->setParameterFloat(FMOD_DSP_ECHO_DRYLEVEL, aDryLevel);
}

/**
 * @brief Set wet level
 * @param aWetLevel
 */
void FMODDSP_Echo::SetWetLevel(float aWetLevel)
{
  GetFMODDSP()->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, aWetLevel);
}
