#include "FMOD_DSP.h"

FMOD_DSP::FMOD_DSP(FMOD::DSP* aDSP, FMODSoundSystem *aSoundSystem, HashString const &aName) : DSP(aName), mDSP(aDSP),
  mSoundSystem(aSoundSystem), mConnectionContainer()
{
}

FMOD_DSP::FMOD_DSP(FMOD::System* aSystem, FMODSoundSystem *aSoundSystem, HashString const &aName, int const &aType) : DSP(aName), 
  mDSP(nullptr), mSoundSystem(aSoundSystem), mConnectionContainer()
{
  FMOD_RESULT result = aSystem->createDSPByType((FMOD_DSP_TYPE)DSP_TYPE_ECHO, &mDSP);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD failed to create DSP");
  }
}

FMOD_DSP::~FMOD_DSP()
{
  mDSP = nullptr;
}

/**
 * @brief Get FMOD DSP
 * @return 
 */
FMOD::DSP* FMOD_DSP::GetFMODDSP()
{
  return mDSP;
}

/**
 * @brief Set DSP formatting
 * @param aNumChannels
 * @param aSpeakerMode
 */
void FMOD_DSP::SetFormat(int aNumChannels, Speaker_Mode const &aSpeakerMode)
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
void FMOD_DSP::AddInput(DSP* aDSP, float **aMixMatrix, int aWidth, int aHeight)
{
  FMOD::DSPConnection *connection;
  mDSP->addInput(mSoundSystem->GetDSP(aDSP->GetName()), &connection);
  connection->setMixMatrix(&aMixMatrix[0][0], aWidth, aHeight);
  mConnectionContainer[(GetName() + aDSP->GetName()).ToHash()] = connection;
}

/**
 * @brief Remove DSP as input
 * @param aInput
 */
void FMOD_DSP::RemoveInput(DSP* aInput)
{
  mDSP->disconnectFrom(mSoundSystem->GetDSP(aInput->GetName()), mConnectionContainer[(GetName() + aInput->GetName()).ToHash()]);
}

/**
 * @brief Set DSP as active
 * @param aActive
 */
void FMOD_DSP::SetActive(bool const aActive)
{
  mDSP->setActive(aActive);
}

/**
 * @brief Set DSP bypass
 * @param aBypass
 */
void FMOD_DSP::SetBypass(bool const aBypass)
{
  mDSP->setBypass(aBypass);
}

/**
 * @brief Set float param, see list of params here: https://www.fmod.com/docs/api/content/generated/FMOD_DSP_SetParameterFloat.html
 * @param aParam https://www.fmod.com/docs/api/content/generated/FMOD_DSP_SetParameterFloat.html
 * @param aValue
 */
void FMOD_DSP::SetFloat(int const aParam, float const aValue)
{
  mDSP->setParameterFloat(aParam, aValue);
}

/**
 * @brief Set int param, see list of params here: https://www.fmod.com/docs/api/content/generated/FMOD_DSP_SetParameterInt.html
 * @param aParam https://www.fmod.com/docs/api/content/generated/FMOD_DSP_SetParameterInt.html
 * @param aValue
 */
void FMOD_DSP::SetInt(int const aParam, int const aValue)
{
  mDSP->setParameterInt(aParam, aValue);
}

/**
 * @brief Set bool param, see list of params here: https://www.fmod.com/docs/api/content/generated/FMOD_DSP_SetParameterBool.html
 * @param aParam https://www.fmod.com/docs/api/content/generated/FMOD_DSP_SetParameterBool.html
 * @param aValue
 */
void FMOD_DSP::SetBool(int const aParam, bool const aValue)
{
  mDSP->setParameterBool(aParam, aValue);
}

/**
 * @brief Set data param, see list of params here: https://www.fmod.com/docs/api/content/generated/FMOD_DSP_SetParameterData.html
 * @param aParam https://www.fmod.com/docs/api/content/generated/FMOD_DSP_SetParameterData.html
 * @param aData
 * @param aLength
 */
void FMOD_DSP::SetData(int const aParam, void* aData, int const aLength)
{
  mDSP->setParameterData(aParam, aData, aLength);
}
