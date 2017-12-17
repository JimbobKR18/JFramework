#include "FMODDSP_Echo.h"

FMODDSP_Echo::FMODDSP_Echo(FMOD::System* aSystem, HashString const &aName) : FMODDSP(aSystem, DSP_TYPE_ECHO, aName), DSP_Echo()
{
}

FMODDSP_Echo::~FMODDSP_Echo()
{
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
