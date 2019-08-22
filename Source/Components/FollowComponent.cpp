#include "FollowComponent.h"
#include "ObjectManager.h"
#include "LevelManager.h"
#include "Level.h"
#include "Transform.h"

int const FollowComponent::sUID = Common::StringHashFunction("FollowComponent");

FollowComponent::FollowComponent() : Component(FollowComponent::sUID), mTarget(nullptr), 
  mTargetName(), mTime(0), mInterpolator(nullptr), mPosition()
{
}

FollowComponent::FollowComponent(FollowComponent const &aFollowComponent) :
  Component(FollowComponent::sUID), mTarget(nullptr),
  mTargetName(aFollowComponent.mTargetName), mTime(0), mInterpolator(nullptr),
  mPosition()
{
}

FollowComponent::~FollowComponent()
{
  ClearInterpolator();
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
  
  if(aTarget)
    mTargetName = aTarget->GetName();
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
  mTarget = nullptr;
}

/**
 * @brief Force component to refind target
 */
void FollowComponent::ResetTarget()
{
  mTarget = nullptr;
}

/**
 * @brief Get travel time to destination
 * @return Travel Time in seconds
 */
float FollowComponent::GetTime() const
{
  return mTime;
}

/**
 * @brief Set travel time to destination
 * @param aTime Time in seconds
 */
void FollowComponent::SetTime(float aTime)
{
  mTime = aTime;
}

// Virtuals derived from Component
/**
 * @brief Update loop.
 */
void FollowComponent::Update()
{
  if(!mTarget && !mTargetName.Empty())
  {
    Level *currentLevel = GetOwner()->GetManager()->GetOwningApp()->GET<LevelManager>()->GetActiveLevel();
    if(currentLevel)
      mTarget = currentLevel->FindObject(mTargetName);
  }
  
  if(mTarget)
  {
    Transform *transform = GetOwner()->GET<Transform>();
    Transform *targetTransform = mTarget->GET<Transform>();
    mPosition = transform->GetPosition();
    
    if(mTime <= 0.01f)
    {
      mPosition = targetTransform->GetPosition();
      ClearInterpolator();
    }
    else
    {
      if(!mInterpolator)
      {
        mInterpolator = new Interpolation<Vector3>(&mPosition, targetTransform->GetPosition(), mTime);
      }
      else
      {
        mInterpolator->Update(mTarget->GetManager()->GetOwningApp()->GetAppStep());
        if(mInterpolator->IsComplete())
        {
          ClearInterpolator();
        }
      }
    }
    transform->SetPosition(mPosition);
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
  }
}

/**
 * @brief Clone FollowComponent
 * @param aNewOwner The new owner
 * @return Cloned FollowComponent
 */
Component* FollowComponent::Clone(GameObject *aNewOwner) const
{
  return new FollowComponent(*this);
}

// Statics
/**
 * @brief Serialize to LUA
 */
void FollowComponent::SerializeLUA()
{
  SLB::Class<FollowComponent>("FollowComponent")
    .inherits<Component>()
    .set("GetTarget", &FollowComponent::GetTarget)
    .set("SetTarget", &FollowComponent::SetTarget)
    .set("GetTargetName", &FollowComponent::GetTargetName)
    .set("SetTargetName", &FollowComponent::SetTargetName);
}

/**
 * @brief Private method to clear interpolator, prevents duplicate code.
 */
void FollowComponent::ClearInterpolator()
{
  if(mInterpolator)
  {
    delete mInterpolator;
    mInterpolator = nullptr;
  }
}
