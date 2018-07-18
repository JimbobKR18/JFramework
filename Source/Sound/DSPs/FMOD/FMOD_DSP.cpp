#include "FMOD_DSP.h"

FMOD_DSP::FMOD_DSP(FMOD::DSP* aDSP, FMODSoundSystem *aSoundSystem, HashString const &aName) : DSP(aName), mDSP(aDSP),
  mSoundSystem(aSoundSystem), mConnectionContainer()
{
}

FMOD_DSP::FMOD_DSP(FMOD::System* aSystem, FMODSoundSystem *aSoundSystem, HashString const &aName, int const &aType) : DSP(aName), 
  mDSP(nullptr), mSoundSystem(aSoundSystem), mConnectionContainer()
{
  FMOD_RESULT result = aSystem->createDSPByType((FMOD_DSP_TYPE)aType, &mDSP);
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

/**
 * @brief Deserialize from node
 * @param aNode Node to deserialize
 */
void FMOD_DSP::Deserialize(ParserNode *aNode)
{
  HashString name = aNode->Find("Name")->GetValue();
  HashString type = aNode->Find("Type")->GetValue();
  
  if(type == "MIXER")
  {
  }
  else if(type == "OSCILLATOR")
  {
    if(aNode->Find("OscillatorType"))
      SetInt(0, aNode->Find("OscillatorType")->GetValue().ToInt());
    if(aNode->Find("OscillatorRate"))
      SetFloat(1, aNode->Find("OscillatorRate")->GetValue().ToFloat());
  }
  else if(type == "LOWPASS")
  {
    if(aNode->Find("LowPassCutOff"))
      SetFloat(0, aNode->Find("LowPassCutOff")->GetValue().ToFloat());
    if(aNode->Find("LowPassResonance"))
      SetFloat(1, aNode->Find("LowPassResonance")->GetValue().ToFloat());
  }
  else if(type == "ITLOWPASS")
  {
    if(aNode->Find("ItLowPassCutOff"))
      SetFloat(0, aNode->Find("ItLowPassCutOff")->GetValue().ToFloat());
    if(aNode->Find("ItLowPassResonance"))
      SetFloat(1, aNode->Find("ItLowPassResonance")->GetValue().ToFloat());
  }
  else if(type == "HIGHPASS")
  {
    if(aNode->Find("HighPassCutOff"))
      SetFloat(0, aNode->Find("HighPassCutOff")->GetValue().ToFloat());
    if(aNode->Find("HighPassResonance"))
      SetFloat(1, aNode->Find("HighPassResonance")->GetValue().ToFloat());
  }
  else if(type == "ECHO")
  {
    if(aNode->Find("EchoDelay"))
      SetFloat(0, aNode->Find("EchoDelay")->GetValue().ToFloat());
    if(aNode->Find("EchoFeedback"))
      SetFloat(1, aNode->Find("EchoFeedback")->GetValue().ToFloat());
    if(aNode->Find("EchoDryLevel"))
      SetFloat(2, aNode->Find("EchoDryLevel")->GetValue().ToFloat());
    if(aNode->Find("EchoWetLevel"))
      SetFloat(3, aNode->Find("EchoWetLevel")->GetValue().ToFloat());
  }
  else if(type == "FADER")
  {
    if(aNode->Find("FaderGain"))
      SetFloat(0, aNode->Find("FaderGain")->GetValue().ToFloat());
  }
  else if(type == "FLANGE")
  {
    if(aNode->Find("FlangeMix"))
      SetFloat(0, aNode->Find("FlangeMix")->GetValue().ToFloat());
    if(aNode->Find("FlangeDepth"))
      SetFloat(1, aNode->Find("FlangeDepth")->GetValue().ToFloat());
    if(aNode->Find("FlangeRate"))
      SetFloat(2, aNode->Find("FlangeRate")->GetValue().ToFloat());
  }
  else if(type == "DISTORTION")
  {
    if(aNode->Find("DistortionLevel"))
      SetFloat(0, aNode->Find("DistortionLevel")->GetValue().ToFloat());
  }
  else if(type == "NORMALIZE")
  {
    if(aNode->Find("NormalizeFadeTime"))
      SetFloat(0, aNode->Find("NormalizeFadeTime")->GetValue().ToFloat());
    if(aNode->Find("NormalizeThreshhold"))
      SetFloat(1, aNode->Find("NormalizeThreshhold")->GetValue().ToFloat());
    if(aNode->Find("NormalizeMaxAmp"))
      SetFloat(2, aNode->Find("NormalizeMaxAmp")->GetValue().ToFloat());
  }
  else if(type == "LIMITER")
  {
    if(aNode->Find("LimiterReleaseTime"))
      SetFloat(0, aNode->Find("LimiterReleaseTime")->GetValue().ToFloat());
    if(aNode->Find("LimiterCeiling"))
      SetFloat(1, aNode->Find("LimiterCeiling")->GetValue().ToFloat());
    if(aNode->Find("LimiterMaximizerGain"))
      SetFloat(2, aNode->Find("LimiterMaximizerGain")->GetValue().ToFloat());
    if(aNode->Find("LimiterMode"))
      SetFloat(3, aNode->Find("LimiterMode")->GetValue().ToFloat());
  }
  else if(type == "PARAMEQ")
  {
    if(aNode->Find("ParamEQCenter"))
      SetFloat(0, aNode->Find("ParamEQCenter")->GetValue().ToFloat());
    if(aNode->Find("ParamEQBandwidth"))
      SetFloat(1, aNode->Find("ParamEQBandwidth")->GetValue().ToFloat());
    if(aNode->Find("ParamEQGain"))
      SetFloat(2, aNode->Find("ParamEQGain")->GetValue().ToFloat());
  }
  else if(type == "PITCHSHIFT")
  {
    if(aNode->Find("PitchShiftPitch"))
      SetFloat(0, aNode->Find("PitchShiftPitch")->GetValue().ToFloat());
    if(aNode->Find("PitchShiftFFTSize"))
      SetFloat(1, aNode->Find("PitchShiftFFTSize")->GetValue().ToFloat());
    if(aNode->Find("PitchShiftOverlap"))
      SetFloat(2, aNode->Find("PitchShiftOverlap")->GetValue().ToFloat());
    if(aNode->Find("PitchShiftMaxChannels"))
      SetFloat(3, aNode->Find("PitchShiftMaxChannels")->GetValue().ToFloat());
  }
  else if(type == "CHORUS")
  {
    if(aNode->Find("ChorusMix"))
      SetFloat(0, aNode->Find("ChorusMix")->GetValue().ToFloat());
    if(aNode->Find("ChorusRate"))
      SetFloat(1, aNode->Find("ChorusRate")->GetValue().ToFloat());
    if(aNode->Find("ChorusDepth"))
      SetFloat(2, aNode->Find("ChorusDepth")->GetValue().ToFloat());
  }
  else if(type == "VSTPLUGIN")
  {
  }
  else if(type == "WINAMPPLUGIN")
  {
  }
  else if(type == "ITECHO")
  {
    if(aNode->Find("ItEchoWetDryMix"))
      SetFloat(0, aNode->Find("ItEchoWetDryMix")->GetValue().ToFloat());
    if(aNode->Find("ItEchoFeedback"))
      SetFloat(1, aNode->Find("ItEchoFeedback")->GetValue().ToFloat());
    if(aNode->Find("ItEchoLeftDelay"))
      SetFloat(2, aNode->Find("ItEchoLeftDelay")->GetValue().ToFloat());
    if(aNode->Find("ItEchoRightDelay"))
      SetFloat(3, aNode->Find("ItEchoRightDelay")->GetValue().ToFloat());
    if(aNode->Find("ItEchoPanDelay"))
      SetFloat(4, aNode->Find("ItEchoPanDelay")->GetValue().ToFloat());
  }
  else if(type == "COMPRESSOR")
  {
    if(aNode->Find("CompressorThreshhold"))
      SetFloat(0, aNode->Find("CompressorThreshhold")->GetValue().ToFloat());
    if(aNode->Find("CompressorRatio"))
      SetFloat(1, aNode->Find("CompressorRatio")->GetValue().ToFloat());
    if(aNode->Find("CompressorAttack"))
      SetFloat(2, aNode->Find("CompressorAttack")->GetValue().ToFloat());
    if(aNode->Find("CompressorGainMakeup"))
      SetFloat(3, aNode->Find("CompressorGainMakeup")->GetValue().ToFloat());
    if(aNode->Find("CompressorUseSideChain"))
    {
      ParserNode *useSideChain = aNode->Find("CompressorUseSideChain");
      FMOD_DSP_PARAMETER_SIDECHAIN sideChain;
      sideChain.sidechainenable = useSideChain->Find("Enable")->GetValue().ToBool();
      SetData(4, &sideChain, sizeof(FMOD_DSP_PARAMETER_SIDECHAIN));
    }
    if(aNode->Find("CompressorLinked"))
      SetBool(5, aNode->Find("CompressorLinked")->GetValue().ToBool());
  }
  else if(type == "SFXREVERB")
  {
    if(aNode->Find("SfxReverbDecayTime"))
      SetFloat(0, aNode->Find("SfxReverbDecayTime")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbEarlyDelay"))
      SetFloat(1, aNode->Find("SfxReverbEarlyDelay")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbLateDelay"))
      SetFloat(2, aNode->Find("SfxReverbLateDelay")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbHFReference"))
      SetFloat(3, aNode->Find("SfxReverbHFReference")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbHFDecayRatio"))
      SetFloat(4, aNode->Find("SfxReverbHFDecayRatio")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbDiffusion"))
      SetFloat(5, aNode->Find("SfxReverbDiffusion")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbDensity"))
      SetFloat(6, aNode->Find("SfxReverbDensity")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbLowShelfFrequency"))
      SetFloat(7, aNode->Find("SfxReverbLowShelfFrequency")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbLowShelfGain"))
      SetFloat(8, aNode->Find("SfxReverbLowShelfGain")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbHighCut"))
      SetFloat(9, aNode->Find("SfxReverbHighCut")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbEarlyLateMix"))
      SetFloat(10, aNode->Find("SfxReverbEarlyLateMix")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbWetLevel"))
      SetFloat(11, aNode->Find("SfxReverbWetLevel")->GetValue().ToFloat());
    if(aNode->Find("SfxReverbDryLevel"))
      SetFloat(12, aNode->Find("SfxReverbDryLevel")->GetValue().ToFloat());
  }
  else if(type == "LOWPASS_SIMPLE")
  {
    if(aNode->Find("LowpassSimpleCutoff"))
      SetFloat(0, aNode->Find("LowpassSimpleCutoff")->GetValue().ToFloat());
  }
  else if(type == "DELAY")
  {
    if(aNode->Find("DelayCh0"))
      SetFloat(0, aNode->Find("DelayCh0")->GetValue().ToFloat());
    if(aNode->Find("DelayCh1"))
      SetFloat(1, aNode->Find("DelayCh1")->GetValue().ToFloat());
    if(aNode->Find("DelayCh2"))
      SetFloat(2, aNode->Find("DelayCh2")->GetValue().ToFloat());
    if(aNode->Find("DelayCh3"))
      SetFloat(3, aNode->Find("DelayCh3")->GetValue().ToFloat());
    if(aNode->Find("DelayCh4"))
      SetFloat(4, aNode->Find("DelayCh4")->GetValue().ToFloat());
    if(aNode->Find("DelayCh5"))
      SetFloat(5, aNode->Find("DelayCh5")->GetValue().ToFloat());
    if(aNode->Find("DelayCh6"))
      SetFloat(6, aNode->Find("DelayCh6")->GetValue().ToFloat());
    if(aNode->Find("DelayCh7"))
      SetFloat(7, aNode->Find("DelayCh7")->GetValue().ToFloat());
    if(aNode->Find("DelayCh8"))
      SetFloat(8, aNode->Find("DelayCh8")->GetValue().ToFloat());
    if(aNode->Find("DelayCh9"))
      SetFloat(9, aNode->Find("DelayCh9")->GetValue().ToFloat());
    if(aNode->Find("DelayCh10"))
      SetFloat(10, aNode->Find("DelayCh10")->GetValue().ToFloat());
    if(aNode->Find("DelayCh11"))
      SetFloat(11, aNode->Find("DelayCh11")->GetValue().ToFloat());
    if(aNode->Find("DelayCh12"))
      SetFloat(12, aNode->Find("DelayCh12")->GetValue().ToFloat());
    if(aNode->Find("DelayCh13"))
      SetFloat(13, aNode->Find("DelayCh13")->GetValue().ToFloat());
    if(aNode->Find("DelayCh14"))
      SetFloat(14, aNode->Find("DelayCh14")->GetValue().ToFloat());
    if(aNode->Find("DelayCh15"))
      SetFloat(15, aNode->Find("DelayCh15")->GetValue().ToFloat());
    if(aNode->Find("DelayMaxDelay"))
      SetFloat(16, aNode->Find("DelayMaxDelay")->GetValue().ToFloat());
  }
  else if(type == "TREMOLO")
  {
    if(aNode->Find("TremoloFrequency"))
      SetFloat(0, aNode->Find("TremoloFrequency")->GetValue().ToFloat());
    if(aNode->Find("TremoloDepth"))
      SetFloat(1, aNode->Find("TremoloDepth")->GetValue().ToFloat());
    if(aNode->Find("TremoloShape"))
      SetFloat(2, aNode->Find("TremoloShape")->GetValue().ToFloat());
    if(aNode->Find("TremoloSkew"))
      SetFloat(3, aNode->Find("TremoloSkew")->GetValue().ToFloat());
    if(aNode->Find("TremoloDuty"))
      SetFloat(4, aNode->Find("TremoloDuty")->GetValue().ToFloat());
    if(aNode->Find("TremoloSquare"))
      SetFloat(5, aNode->Find("TremoloSquare")->GetValue().ToFloat());
    if(aNode->Find("TremoloPhase"))
      SetFloat(6, aNode->Find("TremoloPhase")->GetValue().ToFloat());
    if(aNode->Find("TremoloSpread"))
      SetFloat(7, aNode->Find("TremoloSpread")->GetValue().ToFloat());
  }
  else if(type == "LADSPAPLUGIN")
  {
  }
  else if(type == "SEND")
  {
    if(aNode->Find("SendReturnId"))
      SetInt(0, aNode->Find("SendReturnId")->GetValue().ToInt());
    if(aNode->Find("SendLevel"))
      SetFloat(1, aNode->Find("SendLevel")->GetValue().ToFloat());
  }
  else if(type == "RETURN")
  {
    if(aNode->Find("ReturnId"))
      SetInt(0, aNode->Find("ReturnId")->GetValue().ToInt());
    if(aNode->Find("ReturnInputSpeakerMode"))
      SetInt(1, aNode->Find("ReturnInputSpeakerMode")->GetValue().ToInt());
  }
  else if(type == "HIGHPASS_SIMPLE")
  {
    if(aNode->Find("HighPassSimpleCutoff"))
      SetFloat(0, aNode->Find("HighPassSimpleCutoff")->GetValue().ToFloat());
  }
  else if(type == "PAN")
  {
    if(aNode->Find("PanMode"))
      SetInt(0, aNode->Find("PanMode")->GetValue().ToInt());
    if(aNode->Find("Pan2DStereoPosition"))
      SetFloat(1, aNode->Find("Pan2DStereoPosition")->GetValue().ToFloat());
    if(aNode->Find("Pan2DStereoDirection"))
      SetFloat(2, aNode->Find("Pan2DStereoDirection")->GetValue().ToFloat());
    if(aNode->Find("Pan2DStereoExtent"))
      SetFloat(3, aNode->Find("Pan2DStereoExtent")->GetValue().ToFloat());
    if(aNode->Find("Pan2DStereoRotation"))
      SetFloat(4, aNode->Find("Pan2DStereoRotation")->GetValue().ToFloat());
    if(aNode->Find("Pan2DStereoLFELevel"))
      SetFloat(5, aNode->Find("Pan2DStereoLFELevel")->GetValue().ToFloat());
    if(aNode->Find("Pan2DStereoMode"))
      SetInt(6, aNode->Find("Pan2DStereoMode")->GetValue().ToInt());
    if(aNode->Find("Pan2DStereoSeparation"))
      SetFloat(7, aNode->Find("Pan2DStereoSeparation")->GetValue().ToFloat());
    if(aNode->Find("Pan2DStereoAxis"))
      SetFloat(8, aNode->Find("Pan2DStereoAxis")->GetValue().ToFloat());
    if(aNode->Find("PanEnabledSpeakers"))
      SetInt(9, aNode->Find("PanEnabledSpeakers")->GetValue().ToInt());
    if(aNode->Find("Pan3DPosition"))
    {
      ParserNode *attributesNode = aNode->Find("Pan3DPosition");
      FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI attributes;
      attributes.numlisteners = attributesNode->Find("NumListeners")->GetValue().ToInt();
      // TODO the rest
      
      SetData(10, &attributes, sizeof(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI));
    }
    if(aNode->Find("Pan3DStereoRolloff"))
      SetInt(11, aNode->Find("Pan3DStereoRolloff")->GetValue().ToInt());
    if(aNode->Find("Pan3DStereoMinDistance"))
      SetFloat(12, aNode->Find("Pan3DStereoMinDistance")->GetValue().ToFloat());
    if(aNode->Find("Pan3DStereoMaxDistance"))
      SetFloat(13, aNode->Find("Pan3DStereoMaxDistance")->GetValue().ToFloat());
    if(aNode->Find("Pan3DStereoExtentMode"))
      SetInt(14, aNode->Find("Pan3DStereoExtentMode")->GetValue().ToInt());
    if(aNode->Find("Pan3DStereoSoundSize"))
      SetFloat(15, aNode->Find("Pan3DStereoSoundSize")->GetValue().ToFloat());
    if(aNode->Find("Pan3DStereoMinExtent"))
      SetFloat(16, aNode->Find("Pan3DStereoMinExtent")->GetValue().ToFloat());
    if(aNode->Find("Pan3DStereoPanBlend"))
      SetFloat(17, aNode->Find("Pan3DStereoPanBlend")->GetValue().ToFloat());
    if(aNode->Find("PanLFEUpmixEnabled"))
      SetInt(18, aNode->Find("PanLFEUpmixEnabled")->GetValue().ToInt());
    // Not set by user
    //SetData(19, aNode->Find("PanOverallGain")->GetValue().ToInt());
    if(aNode->Find("PanSurroundSpeakerMode"))
      SetInt(20, aNode->Find("PanSurroundSpeakerMode")->GetValue().ToInt());
    if(aNode->Find("Pan2DHeightBlend"))
      SetFloat(21, aNode->Find("Pan2DHeightBlend")->GetValue().ToFloat());
  }
  else if(type == "THREE_EQ")
  {
    if(aNode->Find("ThreeEQLowGain"))
      SetFloat(0, aNode->Find("ThreeEQLowGain")->GetValue().ToFloat());
    if(aNode->Find("ThreeEQMidGain"))
      SetFloat(1, aNode->Find("ThreeEQMidGain")->GetValue().ToFloat());
    if(aNode->Find("ThreeEQHighGain"))
      SetFloat(2, aNode->Find("ThreeEQHighGain")->GetValue().ToFloat());
    if(aNode->Find("ThreeEQLowCrossover"))
      SetFloat(3, aNode->Find("ThreeEQLowCrossover")->GetValue().ToFloat());
    if(aNode->Find("ThreeEQHighCrossover"))
      SetFloat(4, aNode->Find("ThreeEQHighCrossover")->GetValue().ToFloat());
    if(aNode->Find("ThreeEQCrossoverSlope"))
      SetInt(5, aNode->Find("ThreeEQCrossoverSlope")->GetValue().ToInt());
  }
  else if(type == "FFT")
  {
    if(aNode->Find("FFTWindowSize"))
      SetInt(0, aNode->Find("FFTWindowSize")->GetValue().ToFloat());
    if(aNode->Find("FFTWindowType"))
      SetInt(1, aNode->Find("FFTWindowType")->GetValue().ToFloat());
    if(aNode->Find("FFTSpectrumData"))
    {
      // TODO the rest
      FMOD_DSP_PARAMETER_DATA_TYPE data;
      SetData(2, &data, sizeof(FMOD_DSP_PARAMETER_DATA_TYPE));
    }
    if(aNode->Find("FFTDominantFreq"))
      SetFloat(3, aNode->Find("FFTDominantFreq")->GetValue().ToFloat());
  }
  else if(type == "LOUDNESS_METER")
  {
    // TODO
  }
  else if(type == "ENVELOPEFOLLOWER")
  {
    if(aNode->Find("EnvelopeFollowerAttack"))
      SetFloat(0, aNode->Find("EnvelopeFollowerAttack")->GetValue().ToFloat());
    if(aNode->Find("EnvelopeFollowerRelease"))
      SetFloat(1, aNode->Find("EnvelopeFollowerRelease")->GetValue().ToFloat());
    if(aNode->Find("EnvelopeFollowerEnvelope"))
      SetFloat(2, aNode->Find("EnvelopeFollowerEnvelope")->GetValue().ToFloat());
    if(aNode->Find("EnvelopeFollowerUseSideChain"))
    {
      ParserNode *useSideChain = aNode->Find("EnvelopeFollowerUseSideChain");
      FMOD_DSP_PARAMETER_SIDECHAIN sideChain;
      sideChain.sidechainenable = useSideChain->Find("Enable")->GetValue().ToBool();
      SetData(3, &sideChain, sizeof(FMOD_DSP_PARAMETER_SIDECHAIN));
    }
  }
  else if(type == "CONVOLUTIONREVERB")
  {
  }
  else if(type == "CHANNELMIX")
  {
    if(aNode->Find("ChannelMixGainCh0"))
      SetFloat(0, aNode->Find("ChannelMixGainCh0")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh1"))
      SetFloat(1, aNode->Find("ChannelMixGainCh1")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh2"))
      SetFloat(2, aNode->Find("ChannelMixGainCh2")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh3"))
      SetFloat(3, aNode->Find("ChannelMixGainCh3")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh4"))
      SetFloat(4, aNode->Find("ChannelMixGainCh4")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh5"))
      SetFloat(5, aNode->Find("ChannelMixGainCh5")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh6"))
      SetFloat(6, aNode->Find("ChannelMixGainCh6")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh7"))
      SetFloat(7, aNode->Find("ChannelMixGainCh7")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh8"))
      SetFloat(8, aNode->Find("ChannelMixGainCh8")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh9"))
      SetFloat(9, aNode->Find("ChannelMixGainCh9")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh10"))
      SetFloat(10, aNode->Find("ChannelMixGainCh10")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh11"))
      SetFloat(11, aNode->Find("ChannelMixGainCh11")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh12"))
      SetFloat(12, aNode->Find("ChannelMixGainCh12")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh13"))
      SetFloat(13, aNode->Find("ChannelMixGainCh13")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh14"))
      SetFloat(14, aNode->Find("ChannelMixGainCh14")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh15"))
      SetFloat(15, aNode->Find("ChannelMixGainCh15")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh16"))
      SetFloat(16, aNode->Find("ChannelMixGainCh16")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh17"))
      SetFloat(17, aNode->Find("ChannelMixGainCh17")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh18"))
      SetFloat(18, aNode->Find("ChannelMixGainCh18")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh19"))
      SetFloat(19, aNode->Find("ChannelMixGainCh19")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh20"))
      SetFloat(20, aNode->Find("ChannelMixGainCh20")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh21"))
      SetFloat(21, aNode->Find("ChannelMixGainCh21")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh22"))
      SetFloat(22, aNode->Find("ChannelMixGainCh22")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh23"))
      SetFloat(23, aNode->Find("ChannelMixGainCh23")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh24"))
      SetFloat(24, aNode->Find("ChannelMixGainCh24")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh25"))
      SetFloat(25, aNode->Find("ChannelMixGainCh25")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh26"))
      SetFloat(26, aNode->Find("ChannelMixGainCh26")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh27"))
      SetFloat(27, aNode->Find("ChannelMixGainCh27")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh28"))
      SetFloat(28, aNode->Find("ChannelMixGainCh28")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh29"))
      SetFloat(29, aNode->Find("ChannelMixGainCh29")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh30"))
      SetFloat(30, aNode->Find("ChannelMixGainCh30")->GetValue().ToFloat());
    if(aNode->Find("ChannelMixGainCh31"))
      SetFloat(31, aNode->Find("ChannelMixGainCh31")->GetValue().ToFloat());
  }
  else if(type == "TRANSCEIVER")
  {
    if(aNode->Find("TransceiverTransmit"))
      SetBool(0, aNode->Find("TransceiverTransmit")->GetValue().ToBool());
    if(aNode->Find("TransceiverGain"))
      SetFloat(1, aNode->Find("TransceiverGain")->GetValue().ToFloat());
    if(aNode->Find("TransceiverChannel"))
      SetInt(2, aNode->Find("TransceiverChannel")->GetValue().ToInt());
    if(aNode->Find("TransceiverTransmitSpeakerMode"))
      SetInt(3, aNode->Find("TransceiverTransmitSpeakerMode")->GetValue().ToInt());
  }
  else if(type == "OBJECTPAN")
  {
    if(aNode->Find("ObjectPan3DAttributes"))
    {
      ParserNode *attributesNode = aNode->Find("ObjectPan3DAttributes");
      FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI attributes;
      attributes.numlisteners = attributesNode->Find("NumListeners")->GetValue().ToInt();
      // TODO the rest
      
      SetData(0, &attributes, sizeof(FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI));
    }
    if(aNode->Find("ObjectPan3DRolloff"))
      SetInt(1, aNode->Find("ObjectPan3DRolloff")->GetValue().ToInt());
    if(aNode->Find("ObjectPan3DMinDistance"))
      SetFloat(2, aNode->Find("ObjectPan3DMinDistance")->GetValue().ToFloat());
    if(aNode->Find("ObjectPan3DMaxDistance"))
      SetFloat(3, aNode->Find("ObjectPan3DMaxDistance")->GetValue().ToFloat());
    if(aNode->Find("ObjectPan3DExtentMode"))
      SetInt(4, aNode->Find("ObjectPan3DExtentMode")->GetValue().ToInt());
    if(aNode->Find("ObjectPan3DSoundSize"))
      SetFloat(5, aNode->Find("ObjectPan3DSoundSize")->GetValue().ToFloat());
    if(aNode->Find("ObjectPan3DMinExtent"))
      SetFloat(6, aNode->Find("ObjectPan3DMinExtent")->GetValue().ToFloat());
    // Apparently never set by user
    //SetData(7, aNode->Find("ObjectPanOverallGain")->GetValue().ToBool());
    if(aNode->Find("ObjectPanOutputGain"))
      SetFloat(8, aNode->Find("ObjectPanOutputGain")->GetValue().ToFloat());
  }
  else if(type == "MULTIBAND_EQ")
  {
    if(aNode->Find("MultibandEQAFilter"))
      SetInt(0, aNode->Find("MultibandEQAFilter")->GetValue().ToInt());
    if(aNode->Find("MultibandEQAFrequency"))
      SetFloat(1, aNode->Find("MultibandEQAFrequency")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQAQ"))
      SetFloat(2, aNode->Find("MultibandEQAQ")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQAGain"))
      SetFloat(3, aNode->Find("MultibandEQAGain")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQBFilter"))
      SetInt(4, aNode->Find("MultibandEQBFilter")->GetValue().ToInt());
    if(aNode->Find("MultibandEQBFrequency"))
      SetFloat(5, aNode->Find("MultibandEQBFrequency")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQBQ"))
      SetFloat(6, aNode->Find("MultibandEQBQ")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQBGain"))
      SetFloat(7, aNode->Find("MultibandEQBGain")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQCFilter"))
      SetInt(8, aNode->Find("MultibandEQCFilter")->GetValue().ToInt());
    if(aNode->Find("MultibandEQCFrequency"))
      SetFloat(9, aNode->Find("MultibandEQCFrequency")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQCQ"))
      SetFloat(10, aNode->Find("MultibandEQCQ")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQCGain"))
      SetFloat(11, aNode->Find("MultibandEQCGain")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQDFilter"))
      SetInt(12, aNode->Find("MultibandEQDFilter")->GetValue().ToInt());
    if(aNode->Find("MultibandEQDFrequency"))
      SetFloat(13, aNode->Find("MultibandEQDFrequency")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQDQ"))
      SetFloat(14, aNode->Find("MultibandEQDQ")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQDGain"))
      SetFloat(15, aNode->Find("MultibandEQDGain")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQEFilter"))
      SetInt(16, aNode->Find("MultibandEQEFilter")->GetValue().ToInt());
    if(aNode->Find("MultibandEQEFrequency"))
      SetFloat(17, aNode->Find("MultibandEQEFrequency")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQEQ"))
      SetFloat(18, aNode->Find("MultibandEQEQ")->GetValue().ToFloat());
    if(aNode->Find("MultibandEQEGain"))
      SetFloat(19, aNode->Find("MultibandEQEGain")->GetValue().ToFloat());
  }
  else
  {
    DebugLogPrint("Invalid DSP type %s passed in.", type.ToCharArray());
    assert(!"Invalid DSP type passed in.");
  }
}
