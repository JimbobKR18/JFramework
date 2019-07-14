#include "ChemistryElement.h"
#include "Constants.h"
#include "LUATypes.h"
#include "CollisionMessage.h"

int const ChemistryElement::sUID = Common::StringHashFunction("ChemistryElement");

ChemistryElement::ChemistryElement(ChemistryManager* aManager) : Component(ChemistryElement::sUID), 
  mManager(aManager), mName(), mTemperature(0), mWattage(0), mScale(1.0f), mFalloff(1.0f), 
  mDirectionality()
{
}

ChemistryElement::ChemistryElement(ChemistryElement const &aChemistryElement) : Component(ChemistryElement::sUID),
  mManager(aChemistryElement.mManager), mName(aChemistryElement.mName), mTemperature(aChemistryElement.mTemperature),
  mWattage(aChemistryElement.mWattage), mScale(aChemistryElement.mScale), mFalloff(aChemistryElement.mFalloff),
  mDirectionality(aChemistryElement.mDirectionality)
{
  
}

ChemistryElement::~ChemistryElement()
{
  mManager = nullptr;
}

/**
 * @return Manager
 */
ChemistryManager* ChemistryElement::GetManager() const
{
  return mManager;
}

/**
 * @return Name
 */
HashString const ChemistryElement::GetName() const
{
  return mName;
}

/**
 * @return Temperature.
 */
float const ChemistryElement::GetTemperature() const
{
  return mTemperature;
}

/**
 * @return Wattage. 
 */
float const ChemistryElement::GetWattage() const
{
  return mWattage;
}

/**
 * @return Scaling.
 */
float const ChemistryElement::GetScale() const
{
  return mScale;
}

/**
 * @return Falloff rate of effect.
 */
float const ChemistryElement::GetFalloff() const
{
  return mFalloff;
}

/**
 * @return Direction of element.
 */
Vector3 const &ChemistryElement::GetDirectionality() const
{
  return mDirectionality;
}

/**
 * @brief Set name of element.
 * @param aName Name.
 */
void ChemistryElement::SetName(HashString const &aName)
{
  mName = aName;
}

/**
 * @brief Set temperature of element.
 * @param aTemperature Temperature.
 */
void ChemistryElement::SetTemperature(float const aTemperature)
{
  mTemperature = aTemperature;
}

/**
 * @brief Set wattage of element.
 * @param aWattage Wattage.
 */
void ChemistryElement::SetWattage(float const aWattage)
{
  mWattage = aWattage;
}

/**
 * @brief Set scale of element.
 * @param aScale Scale.
 */
void ChemistryElement::SetScale(float const aScale)
{
  mScale = aScale;
}

/**
 * @brief Set falloff of element.
 * @param aFalloff Falloff.
 */
void ChemistryElement::SetFalloff(float const aFalloff)
{
  mFalloff = aFalloff;
}

/**
 * @brief Set direction of element.
 * @param aDirectionality Direction.
 */
void ChemistryElement::SetDirectionality(Vector3 const &aDirectionality)
{
  mDirectionality = aDirectionality;
}

/**
 * @brief Update loop.
 */
void ChemistryElement::Update()
{
}

/**
 * @brief Send message to upper levels.
 * @param aMessage Message to send.
 */
void ChemistryElement::SendMessage(Message const& aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

/**
 * @brief Receive message from above.
 * @param aMessage Message to receive.
 */
void ChemistryElement::ReceiveMessage(Message const& aMessage)
{
  if(aMessage.GetDescription() != COLLISION)
    return;

  CollisionMessage *message = (CollisionMessage*)&aMessage;
  GameObject *otherBody = message->GetObject(0) != GetOwner() ?
                          message->GetObject(0) : message->GetObject(1);
  
  if(otherBody->HAS<ChemistryElement>())
  {
    ChemistryElement *otherChemical = otherBody->GET<ChemistryElement>();
    Vector3 direction = message->GetObject(0) != GetOwner() ? 
                        message->GetNormal() : message->GetNormal().Invert();
                        
    // TODO do this better
    float otherTemperature = otherChemical->GetTemperature();
    float otherWattage = otherChemical->GetWattage();
    
    if(otherTemperature < mTemperature)
    {
      mTemperature = otherTemperature;
    }
    if(otherWattage > mWattage)
    {
      mWattage = otherWattage;
    }
    
    HashString luaFunctionName = mName + "_" + otherChemical->GetName() + "_Reaction";
    if(!LUABind::LoadFunction<ChemistryElement*>("ChemistryMessages.lua",
                                     luaFunctionName,
                                     this, otherChemical))
    {
      DebugLogPrint("No chemistry function %s to call in LUA.\n", luaFunctionName.ToCharArray());
    }
  }
}

/**
 * @brief Serialize out.
 * @param aParser Parser to write to.
 */
void ChemistryElement::Serialize(ParserNode *aNode)
{
  HashString const CHEMISTRY_OBJECT = "ChemistryElement";
  
  aNode->Place(CHEMISTRY_OBJECT, "");
  ParserNode* object = aNode->Find(CHEMISTRY_OBJECT);
  
  object->Place("Name", mName);
  object->Place("Temperature", Common::FloatToString(mTemperature));
  object->Place("Wattage", Common::FloatToString(mWattage));
  object->Place("Scale", Common::FloatToString(mScale));
  object->Place("Falloff", Common::FloatToString(mFalloff));
  object->Place("DirectionX", Common::FloatToString(mDirectionality.x));
  object->Place("DirectionY", Common::FloatToString(mDirectionality.y));
  object->Place("DirectionZ", Common::FloatToString(mDirectionality.z));
}

/**
 * @brief Deserialize from parser into memory.
 * @param aNode ParserNode to read from.
 */
void ChemistryElement::Deserialize(ParserNode *aNode)
{
  if(aNode->Find("Name"))
    mName = aNode->Find("Name")->GetValue();
  if(aNode->Find("Temperature"))
    mTemperature = aNode->Find("Temperature")->GetValue().ToFloat();
  if(aNode->Find("Wattage"))
    mWattage = aNode->Find("Wattage")->GetValue().ToFloat();
  if(aNode->Find("Scale"))
    mScale = aNode->Find("Scale")->GetValue().ToFloat();
  if(aNode->Find("Falloff"))
    mFalloff = aNode->Find("Falloff")->GetValue().ToFloat();
  if(aNode->Find("DirectionX"))
    mDirectionality.x = aNode->Find("DirectionX")->GetValue().ToFloat();
  if(aNode->Find("DirectionY"))
    mDirectionality.y = aNode->Find("DirectionY")->GetValue().ToFloat();
  if(aNode->Find("DirectionZ"))
    mDirectionality.z = aNode->Find("DirectionZ")->GetValue().ToFloat();
}

/**
 * @brief Clone ChemistryElement
 * @param aNewOwner The new owner
 * @return Cloned ChemistryElement
 */
Component* ChemistryElement::Clone(GameObject *aNewOwner) const
{
  return new ChemistryElement(*this);
}

/**
 * @brief Serialize to LUA.
 */
void ChemistryElement::SerializeLUA()
{
  SLB::Class<ChemistryElement>("ChemistryElement")
    .inherits<Component>();
}
