#include "FMODSoundSystem.h"

FMODSoundSystem::FMODSoundSystem() : mFMODStudioSystem(nullptr), mFMODSystem(nullptr), 
                                     mMasterChannelGroup(nullptr), mSoundContainer()
{
  FMOD_RESULT result = FMOD::Studio::System::create(&mFMODStudioSystem);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD Studio error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD Studio failed to initialize.");
  }
  
  result = mFMODStudioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD failed to initialize.");
  }
  
  mFMODStudioSystem->getLowLevelSystem(&mFMODSystem);
  mFMODSystem->getMasterChannelGroup(&mMasterChannelGroup);
}

FMODSoundSystem::~FMODSoundSystem()
{
  mFMODStudioSystem->release();
}

void FMODSoundSystem::CreateSound(HashString const& aFilename, HashString const& aAlias)
{
  HashString fileName = Common::RelativePath("Sounds", aFilename);
  FMOD::Sound *sound;
  
  FMOD_RESULT result = mFMODSystem->createSound(fileName.ToCharArray(), FMOD_DEFAULT, 0, &sound);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD failed to create sound.");
  }
  
  if(!aAlias.Empty())
  {
    mSoundContainer[aAlias.ToHash()] = sound;
  }
  else
  {
    mSoundContainer[aFilename.ToHash()] = sound;
  }
}

void FMODSoundSystem::DeleteSound(HashString const& aName)
{
  mSoundContainer[aName.ToHash()]->release();
  mSoundContainer.erase(aName.ToHash());
}

int FMODSoundSystem::FadeInSound(HashString const& aName, int const aNumLoops, int const aTime)
{
  FMOD::Channel *channel;
  FMOD_RESULT result = mFMODSystem->playSound(mSoundContainer[aName.ToHash()], 0, false, &channel);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD failed to play sound.");
  }
  
  // TODO delete.
  FMOD::DSP* dsp;
  mFMODSystem->createDSPByType(FMOD_DSP_TYPE_FADER, &dsp);
  channel->addDSP(0, dsp);
}

void FMODSoundSystem::PauseSound(int const aChannel)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->setPaused(true);
}

int FMODSoundSystem::PlaySound(HashString const& aName, int const aNumLoops)
{
  FMOD::Channel *channel;
  FMOD_RESULT result = mFMODSystem->playSound(mSoundContainer[aName.ToHash()], 0, false, &channel);
  if(result != FMOD_OK)
  {
    DebugLogPrint("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
    assert(!"FMOD failed to play sound.");
  }
}

void FMODSoundSystem::ResumeSound(int const aChannel)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->setPaused(false);
}

void FMODSoundSystem::SetChannelVolume(int const aChannel, float const aVolume)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->setVolume(aVolume);
}

void FMODSoundSystem::SetVolume(float const aVolume)
{
  mMasterChannelGroup->setVolume(aVolume);
}

void FMODSoundSystem::StopSound(int const aChannel)
{
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->stop();
}

void FMODSoundSystem::FadeOutSound(int const aChannel, int const aMillis)
{
  // TODO
  FMOD::Channel *channel;
  mFMODSystem->getChannel(aChannel, &channel);
  channel->stop();
  
  // TODO delete.
  FMOD::DSP* dsp;
  mFMODSystem->createDSPByType(FMOD_DSP_TYPE_FADER, &dsp);
  channel->addDSP(0, dsp);
}

void FMODSoundSystem::Update(float const aDT)
{
  mFMODStudioSystem->update();
}
