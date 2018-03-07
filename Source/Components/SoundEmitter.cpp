#include "SoundEmitter.h"
#include "Transform.h"
#include "SoundManager.h"
#include "ObjectManager.h"
#include "CollisionMessage.h"
#include "LUATypes.h"

int const SoundEmitter::sUID = Common::StringHashFunction("SoundEmitter");

SoundEmitter::SoundEmitter() : Component(SoundEmitter::sUID), mPattern(SoundEmitPattern::CONSTANT),
  mSoundName(), mSoundOrigin(), mChannel(-1), mVolume(0), mListeners()
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
 * @brief Get name of sound that will play.
 * @return Name of sound.
 */
HashString SoundEmitter::GetSoundName() const
{
  return mSoundName;
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
 * @brief Set sound emit pattern.
 * @param aPattern Sound emit pattern.
 */
void SoundEmitter::SetSoundEmitPattern(SoundEmitPattern const &aPattern)
{
  mPattern = aPattern;
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
 * @brief Set location of where sound plays from.
 * @param aSoundOrigin Origin point in local space.
 */
void SoundEmitter::SetSoundOrigin(Vector3 const &aSoundOrigin)
{
  mSoundOrigin = aSoundOrigin;
}

/**
 * @brief Stop playing sound.
 */
void SoundEmitter::StopSound()
{
  SoundManager *soundManager = GetOwner()->GetManager()->GetOwningApp()->GET<SoundManager>();
  soundManager->StopChannel(mChannel);
  mChannel = -1;
}

/**
 * @brief Update loop.
 */
void SoundEmitter::Update()
{
  if(mListeners.size() == 0)
  {
    StopSound();
  }
  mVolume = 0;
  mListeners.clear();
}

/**
 * @brief Receive message
 * @param aMessage Message to receive
 */
void SoundEmitter::ReceiveMessage(Message const& aMessage)
{
  if(aMessage.GetDescription() != "Collision")
    return;

  CollisionMessage *message = (CollisionMessage*)&aMessage;
  GameObject *notOwner = message->GetObject(0) == GetOwner() ? message->GetObject(1) : message->GetObject(0);
  SoundManager *soundManager = notOwner->GetManager()->GetOwningApp()->GET<SoundManager>();
  
  if(notOwner->HAS<SoundListener>())
  {
    Transform *ownerTransform = GetOwner()->GET<Transform>();
    Transform *notOwnerTransform = notOwner->GET<Transform>();
    Vector3 soundFinalPosition = ownerTransform->GetHierarchicalPosition() + mSoundOrigin;
    float volume = 1.0f;
    switch(mPattern)
    {
      case CONSTANT:
        volume = 1.0f;
        break;
      case HORIZONTAL_LINE:
        volume = 1.0f - (fabs(notOwnerTransform->GetPosition().y - soundFinalPosition.y) / notOwnerTransform->GetSize().y);
        break;
      case VERTICAL_LINE:
        volume = 1.0f - (fabs(notOwnerTransform->GetPosition().x - soundFinalPosition.x) / notOwnerTransform->GetSize().x);
        break;
      case RADIAL:
        volume = 1.0f - ((notOwnerTransform->GetPosition() - soundFinalPosition).length() / notOwnerTransform->GetSize().length());
        break;
    }
    
    if(mChannel == -1)
      mChannel = soundManager->PlaySound(mSoundName, SoundManager::INFINITE_LOOPS);
    
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

  if(mPattern == SoundEmitPattern::CONSTANT)
    soundEmitter->Place("Pattern", "CONSTANT");
  else if(mPattern == SoundEmitPattern::HORIZONTAL_LINE)
    soundEmitter->Place("Pattern", "HORIZONTAL_LINE");
  else if(mPattern == SoundEmitPattern::VERTICAL_LINE)
    soundEmitter->Place("Pattern", "VERTICAL_LINE");
  else if(mPattern == SoundEmitPattern::RADIAL)
    soundEmitter->Place("Pattern", "RADIAL");
  
  HashString originString = Common::FloatToString(mSoundOrigin.x) + "," +
                            Common::FloatToString(mSoundOrigin.y) + "," +
                            Common::FloatToString(mSoundOrigin.z);
  soundEmitter->Place("SoundName", mSoundName);
  soundEmitter->Place("SoundOrigin", originString);
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
  if(aNode->Find("Pattern"))
  {
    HashString pattern = aNode->Find("Pattern")->GetValue();
    if(pattern == "CONSTANT")
      mPattern = SoundEmitPattern::CONSTANT;
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
