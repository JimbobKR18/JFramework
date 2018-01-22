#include "FollowComponent.h"
#include "ObjectManager.h"
#include "LevelManager.h"
#include "Level.h"
#include "Transform.h"

int const FollowComponent::sUID = Common::StringHashFunction("FollowComponent");

FollowComponent::FollowComponent() : Component(FollowComponent::sUID), mTarget(nullptr), mTargetName()
{
}

FollowComponent::~FollowComponent()
{
}

// Methods
/**
 * @brief Get target
 * @return Target
 */
GameObject *FollowComponent::GetTarget()
{
  return mTarget;
}

/**
 * @brief Set target
 * @param aTarget Target
 */
void FollowComponent::SetTarget(GameObject *aTarget)
{
  mTarget = aTarget;
}

/**
 * @brief Get name of target
 * @return Target name
 */
HashString FollowComponent::GetTargetName() const
{
  return mTargetName;
}

/**
 * @brief Set target name
 * @param aTargetName Target name
 */
void FollowComponent::SetTargetName(HashString const &aTargetName)
{
  mTargetName = aTargetName;
}

// Virtuals derived from Component
/**
 * @brief Update loop.
 */
void FollowComponent::Update()
{
  Transform *transform = GetOwner()->GET<Transform>();
  
  if(!mTarget && !mTargetName.Empty())
  {
    Level *currentLevel = GetOwner()->GetManager()->GetOwningApp()->GET<LevelManager>()->GetActiveLevel();
    if(currentLevel)
      mTarget = currentLevel->FindObject(mTargetName);
  }
  
  if(mTarget)
  {
    Transform *targetTransform = mTarget->GET<Transform>();
    transform->SetPosition(targetTransform->GetPosition());
  }
}

/**
 * @brief Does nothing
 * @param aMessage
 */
void FollowComponent::SendMessage(Message const& aMessage)
{
  
}

/**
 * @brief Does nothing
 * @param aMessage
 */
void FollowComponent::ReceiveMessage(Message const& aMessage)
{
  
}

/**
 * @brief Serialize to file
 * @param aNode Node to write to
 */
void FollowComponent::Serialize(ParserNode *aNode)
{
  HashString const FOLLOW_COMPONENT = "FollowComponent";
  aNode->Place(FOLLOW_COMPONENT, "");
  ParserNode* object = aNode->Find(FOLLOW_COMPONENT);
  
  if(mTarget)
    object->Place("Target", mTarget->GetName());
}

/**
 * @brief Deserialize from file
 * @param aNode
 */
void FollowComponent::Deserialize(ParserNode *aNode)
{
  if(aNode->Find("Target"))
  {
    mTargetName = aNode->Find("Target")->GetValue();
    Level *currentLevel = GetOwner()->GetManager()->GetOwningApp()->GET<LevelManager>()->GetActiveLevel();
    
    if(currentLevel)
      mTarget = currentLevel->FindObject(mTargetName);
  }
}

// Statics
/**
 * @brief Serialize to LUA
 */
void FollowComponent::SerializeLUA()
{
  SLB::Class<FollowComponent>("FollowComponent")
    .set("GetTarget", &FollowComponent::GetTarget)
    .set("SetTarget", &FollowComponent::SetTarget)
    .set("GetTargetName", &FollowComponent::GetTargetName)
    .set("SetTargetName", &FollowComponent::SetTargetName);
}
