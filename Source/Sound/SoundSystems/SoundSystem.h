#ifndef __JFramework_SoundSystem_h_
#define __JFramework_SoundSystem_h_

#include "Common.h"
#include "MathExt.h"
#include "DSP_Type.h"
#include "DSP.h"

class SoundSystem
{
public:
  enum SoundSource
  {
    DEFAULT = 0,
    STREAM
  };

  SoundSystem();
  virtual ~SoundSystem();
  
  // Update
  virtual void Update(float const aDT) = 0;
  
  // Sound init and play init
  virtual void CreateSound(HashString const& aFilename, HashString const& aAlias, float const &aDefaultVolume, SoundSource const& aSource) = 0;
  virtual void DeleteSound(HashString const& aName) = 0;
  virtual int PlaySound(HashString const& aName, int const aNumLoops) = 0;
  
  // Channels
  virtual void ResumeChannel(int const aChannel) = 0;
  virtual void PauseChannel(int const aChannel) = 0;
  virtual void SetMasterVolume(float const aVolume) = 0;
  virtual void StopAllChannels() = 0;
  virtual void SetChannelVolume(int const aChannel, float const aVolume) = 0;
  virtual void StopChannel(int const aChannel) = 0;
  virtual void FadeChannel(int const aChannel, int const aTime, float const aStart, float const aEnd) = 0;
  virtual void SetChannelFrequency(int const aChannel, float const aFrequency) = 0;
  virtual void SetChannel3DAttributes(int const aChannel, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos) = 0;
  virtual void SetChannel3DCone(int const aChannel, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume) = 0;
  virtual void SetChannel3DAttenuation(int const aChannel, std::vector<Vector3> const &aPoints) = 0;
  virtual void SetChannel3DMinMaxDistance(int const aChannel, float const aMinDistance, float const aMaxDistance) = 0;
  virtual void SetChannel3DOcclusion(int const aChannel, float const aDirectOcclusion, float const aReverbOcclusion) = 0;
  virtual void SetChannel3DSpread(int const aChannel, float const aAngle) = 0;
  
  // Channel groups
  virtual void CreateChannelGroup(HashString const &aGroupName) = 0;
  virtual void AddChannelToGroup(HashString const &aGroupName, int const aChannel) = 0;
  virtual void SetChannelGroupVolume(HashString const &aGroupName, float const aVolume) = 0;
  virtual void StopChannelGroup(HashString const &aGroupName) = 0;
  virtual void FadeChannelGroup(HashString const &aGroupName, int const aTime, float const aStart, float const aEnd) = 0;
  virtual void SetChannelGroup3DAttributes(HashString const &aGroupName, Vector3 const &aPos, Vector3 const &aVel, Vector3 const &aAltPanPos) = 0;
  virtual void SetChannelGroup3DCone(HashString const &aGroupName, Vector3 const &aOrientation, float const aInsideAngle, float const aOutsideAngle, float const aOutsideVolume) = 0;
  virtual void SetChannelGroup3DAttenuation(HashString const &aGroupName, std::vector<Vector3> const &aPoints) = 0;
  virtual void SetChannelGroup3DMinMaxDistance(HashString const &aGroupName, float const aMinDistance, float const aMaxDistance) = 0;
  virtual void SetChannelGroup3DOcclusion(HashString const &aGroupName, float const aDirectOcclusion, float const aReverbOcclusion) = 0;
  virtual void SetChannelGroup3DSpread(HashString const &aGroupName, float const aAngle) = 0;
  
  // DSPs
  virtual DSP* CreateDSP(HashString const &aName, DSP_Type const &aType) = 0;
  virtual DSP* GetDSPFromChannel(int aChannel, int aIndex) = 0;
  virtual DSP* GetDSPFromChannelGroup(HashString const &aGroupName, int aIndex) = 0;
  virtual bool DeleteDSP(DSP *aDSP) = 0;
  virtual void AddDSPToChannel(DSP *aDSP, int const aChannel, int const aIndex) = 0;
  virtual void AddDSPToChannelGroup(DSP *aDSP, HashString const& aGroupName, int const aIndex) = 0;
  virtual void RemoveDSPFromChannel(DSP *aDSP, int const aChannel) = 0;
  virtual void RemoveDSPFromChannelGroup(DSP *aDSP, HashString const& aGroupName) = 0;
};

#endif // __JFramework_SoundSystem_h_
