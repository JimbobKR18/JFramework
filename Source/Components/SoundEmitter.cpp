#include "SoundEmitter.h"
#include "Transform.h"
#include "SoundManager.h"
#include "ObjectManager.h"
#include "CollisionMessage.h"
#include "LUATypes.h"

HashType const SoundEmitter::sUID = Common::StringHashFunction("SoundEmitter");

SoundEmitter::SoundEmitter() : Component(SoundEmitter::sUID), mPattern(SoundEmitPattern::UNIFORM),
  mFunction(SoundEmitFunction::CONSTANT), mSoundName(), mChannelGroup(), mSoundOrigin(), mChannel(-1), mVolume(0), 
  mActive(true), mListeners()
{
}

SoundEmitter::SoundEmitter(SoundEmitter const &aSoundEmitter) : Component(SoundEmitter::sUID),
  mPattern(aSoundEmitter.mPattern), mFunction(aSoundEmitter.mFunction), mSoundName(aSoundEmitter.mSoundName), 
  mChannelGroup(aSoundEmitter.mChannelGroup), mSoundOrigin(aSoundEmitter.mSoundOrigin), mChannel(aSoundEmitter.mChannel), 
  mVolume(aSoundEmitter.mVolume), mActive(true), mListeners()
{
}

SoundEmitter::~SoundEmitter()
{
}

/**
 * @brief Get emit pattern for emitter.
 * @return Emit pattern.
 */
SoundEmitPattern SoundEmitter::GetSoundEmitPattern() const
{
  return mPattern;
}

/**
 * @brief Get emit function for emitter.
 * @return Emit function.
 */
SoundEmitFunction SoundEmitter::GetSoundEmitFunction() const
{
  return mFunction;
}

/**
 * @brief Get name of sound that will play.
 * @return Name of sound.
 */
HashString SoundEmitter::GetSoundName() const
{
  return mSoundName;
}

/**
 * @brief Get group of channel.
 * @return Name of group.
 */
HashString SoundEmitter::GetChannelGroup() const
{
  return mChannelGroup;
}

/**
 * @brief Get origin point of sound for emitter.
 * @return Origin point, in local space.
 */
Vector3 SoundEmitter::GetSoundOrigin() const
{
  return mSoundOrigin;
}

/**
 * @brief Get channel sound is playing on.
 * @return Channel sound is playing on, if not playing, returns -1.
 */
int SoundEmitter::GetChannel() const
{
  return mChannel;
}

/**
 * @brief Get active state.
 * @return True if active.
 */
bool SoundEmitter::GetActive() const
{
  return mActive;
}

/**
 * @brief Set sound emit pattern.
 * @param aPattern Sound emit pattern.
 */
void SoundEmitter::SetSoundEmitPattern(SoundEmitPattern const &aPattern)
{
  mPattern = aPattern;
}

/**
 * @brief Set sound emit function.
 * @param aFunction Sound emit function.
 */
void SoundEmitter::SetSoundEmitFunction(SoundEmitFunction const &aFunction)
{
  mFunction = aFunction;
}

/**
 * @brief Set sound name.
 * @param aSoundName Name of sound.
 */
void SoundEmitter::SetSoundName(HashString const &aSoundName)
{
  mSoundName = aSoundName;
}

/**
 * @brief Set sound channel group.
 * @param aChannelGroup Name of group.
 */
void SoundEmitter::SetChannelGroup(HashString const &aChannelGroup)
{
  mChannelGroup = aChannelGroup;
}

/**
 * @brief Set location of where sound plays from.
 * @param aSoundOrigin Origin point in local space.
 */
void SoundEmitter::SetSoundOrigin(Vector3 const &aSoundOrigin)
{
  mSoundOrigin = aSoundOrigin;
}

/**
 * @brief Set sound active status.
 * @param aActive Sound active status.
 */
void SoundEmitter::SetActive(bool const &aActive)
{
  mActive = aActive;
}

/**
 * @brief Stop playing sound.
 */
void SoundEmitter::StopSound()
{
  if(mChannel == -1)
    return;
    
  SoundManager *soundManager = GetOwner()->GetManager()->GetOwningApp()->GET<SoundManager>();
  soundManager->StopChannel(mChannel);
  mChannel = -1;
}

/**
 * @brief Update loop.
 */
void SoundEmitter::Update()
{
  mVolume = 0;
  if(mListeners.size() == 0 && mChannel != -1)
  {
    SoundManager *soundManager = GetOwner()->GetManager()->GetOwningApp()->GET<SoundManager>();
    soundManager->SetChannelVolume(mChannel, mVolume);
  }
  mListeners.clear();
}

/**
 * @brief Receive message
 * @param aMessage Message to receive
 */
void SoundEmitter::ReceiveMessage(Message const& aMessage)
{
  if(aMessage.GetDescription() != COLLISION || !mActive)
    return;

  CollisionMessage *message = (CollisionMessage*)&aMessage;
  GameObject *notOwner = message->GetObject(0) == GetOwner() ? message->GetObject(1) : message->GetObject(0);
  SoundManager *soundManager = notOwner->GetManager()->GetOwningApp()->GET<SoundManager>();
  
  if(notOwner->HAS<SoundListener>())
  {
    if(!notOwner->GET<SoundListener>()->GetActive())
      return;
    
    Transform *ownerTransform = GetOwner()->GET<Transform>();
    Transform *notOwnerTransform = notOwner->GET<Transform>();
    Vector3 soundFinalPosition = ownerTransform->GetHierarchicalPosition() + mSoundOrigin;
    Vector3 ownerFinalSize = ownerTransform->GetSize().Multiply(ownerTransform->GetHierarchicalScale());
    float volume = 1.0f;
    switch(mPattern)
    {
      case UNIFORM:
        volume = 1.0f;
        break;
      case HORIZONTAL_LINE:
        volume = (fabs(notOwnerTransform->GetPosition().y - soundFinalPosition.y) / ownerFinalSize.y);
        break;
      case VERTICAL_LINE:
        volume = (fabs(notOwnerTransform->GetPosition().x - soundFinalPosition.x) / ownerFinalSize.x);
        break;
      case RADIAL:
        volume = ((notOwnerTransform->GetPosition() - soundFinalPosition).length() / ownerFinalSize.length());
        break;
    }
    
    switch(mFunction)
    {
      case CONSTANT:
        break;
      case LINEAR:
        volume = 1.0f - volume;
        break;
      case SQUARED:
        volume = volume * volume;
        break;
      case INVERSE_SQUARED:
        volume = 1.0f - (volume * volume);
        break;
    }
    
    if(mChannel == -1)
    {
      mChannel = soundManager->PlaySound(mSoundName, SoundManager::INFINITE_LOOPS);
      if(!mChannelGroup.Empty())
        soundManager->AddChannelToGroup(mChannelGroup, mChannel);
    }
    
    if(volume > mVolume)
      mVolume = volume;
    
    soundManager->SetChannelVolume(mChannel, mVolume);
    mListeners.insert(notOwner->GET<SoundListener>());
  }
}

/**
 * @brief Send message
 * @param aMessage Message to send
 */
void SoundEmitter::SendMessage(Message const& aMessage)
{
}

/**
 * @brief Serialize to node
 * @param aNode Node to serialize to
 */
void SoundEmitter::Serialize(ParserNode* aNode)
{
  HashString const SOUNDEMITTER = "SoundEmitter";
  aNode->Place(SOUNDEMITTER, "");
  ParserNode* soundEmitter = aNode->Find(SOUNDEMITTER);

  if(mPattern == SoundEmitPattern::UNIFORM)
    soundEmitter->Place("Pattern", "UNIFORM");
  else if(mPattern == SoundEmitPattern::HORIZONTAL_LINE)
    soundEmitter->Place("Pattern", "HORIZONTAL_LINE");
  else if(mPattern == SoundEmitPattern::VERTICAL_LINE)
    soundEmitter->Place("Pattern", "VERTICAL_LINE");
  else if(mPattern == SoundEmitPattern::RADIAL)
    soundEmitter->Place("Pattern", "RADIAL");
    
  if(mFunction == SoundEmitFunction::CONSTANT)
    soundEmitter->Place("Function", "UNIFORM");
  else if(mFunction == SoundEmitFunction::LINEAR)
    soundEmitter->Place("Function", "LINEAR");
  else if(mFunction == SoundEmitFunction::SQUARED)
    soundEmitter->Place("Function", "SQUARED");
  else if(mFunction == SoundEmitFunction::INVERSE_SQUARED)
    soundEmitter->Place("Function", "INVERSE_SQUARED");
  
  HashString originString = Common::FloatToString(mSoundOrigin.x) + "," +
                            Common::FloatToString(mSoundOrigin.y) + "," +
                            Common::FloatToString(mSoundOrigin.z);
  soundEmitter->Place("SoundName", mSoundName);
  soundEmitter->Place("SoundOrigin", originString);
  soundEmitter->Place("Active", Common::BoolToString(mActive));
  
  if(!mChannelGroup.Empty())
    soundEmitter->Place("Group", mChannelGroup);
}

/**
 * @brief Deserialize from node
 * @param aNode Node to read from
 */
void SoundEmitter::Deserialize(ParserNode* aNode)
{
  if(aNode->Find("SoundName"))
    mSoundName = aNode->Find("SoundName")->GetValue();
  if(aNode->Find("SoundOrigin"))
    mSoundOrigin = aNode->Find("SoundOrigin")->GetValue().ToVector3();
  if(aNode->Find("Active"))
    mActive = aNode->Find("mActive")->GetValue().ToBool();
  if(aNode->Find("Group"))
    mChannelGroup = aNode->Find("Group")->GetValue();
  if(aNode->Find("Pattern"))
  {
    HashString pattern = aNode->Find("Pattern")->GetValue();
    if(pattern == "UNIFORM")
      mPattern = SoundEmitPattern::UNIFORM;
    else if(pattern == "HORIZONTAL_LINE")
      mPattern = SoundEmitPattern::HORIZONTAL_LINE;
    else if(pattern == "VERTICAL_LINE")
      mPattern = SoundEmitPattern::VERTICAL_LINE;
    else if(pattern == "RADIAL")
      mPattern = SoundEmitPattern::RADIAL;
    else
    {
      DebugLogPrint("Invalid SoundEmitPattern type %s passed into SoundEmitter Deserialize.\n", pattern.ToCharArray());
      assert(!"Invalid SoundEmitPattern type passed into SoundEmitter Deserialize.");
    }
  }
  if(aNode->Find("Function"))
  {
    HashString function = aNode->Find("Function")->GetValue();
    if(function == "CONSTANT")
      mFunction = SoundEmitFunction::CONSTANT;
    else if(function == "LINEAR")
      mFunction = SoundEmitFunction::LINEAR;
    else if(function == "SQUARED")
      mFunction = SoundEmitFunction::SQUARED;
    else if(function == "INVERSE_SQUARED")
      mFunction = SoundEmitFunction::INVERSE_SQUARED;
    else
    {
      DebugLogPrint("Invalid SoundEmitFunction type %s passed into SoundEmitter Deserialize.\n", function.ToCharArray());
      assert(!"Invalid SoundEmitFunction type passed into SoundEmitter Deserialize.");
    }
  }
}

/**
 * @brief Clone SoundEmitter
 * @param aNewOwner The new owner
 * @return Cloned SoundEmitter
 */
Component* SoundEmitter::Clone(GameObject *aNewOwner) const
{
  return new SoundEmitter(*this);
}

/**
 * @brief Make this object usable in LUA
 */
void SoundEmitter::SerializeLUA()
{
  SLB::Class<SoundEmitter>("SoundEmitter")
    .inherits<Component>()
    .set("GetSoundName", &SoundEmitter::GetSoundName)
    .set("GetSoundOrigin", &SoundEmitter::GetSoundOrigin)
    .set("GetChannel", &SoundEmitter::GetChannel)
    .set("SetSoundName", &SoundEmitter::SetSoundName)
    .set("SetSoundOrigin", &SoundEmitter::SetSoundOrigin);
}
