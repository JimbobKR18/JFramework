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

ChemistryElement::~ChemistryElement()
{
  mManager->RemoveElement(this);
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
  if(aMessage.GetDescription() != "Collision")
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
void ChemistryElement::Serialize(Parser& aParser)
{
  HashString const objectName = HashString("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  HashString const CHEMISTRY_OBJECT = "ChemistryElement";
  ParserNode* object = aParser.Find(objectName);
  
  object->Place(objectName, CHEMISTRY_OBJECT, "");
  object->Place(CHEMISTRY_OBJECT, "Name", mName);
  object->Place(CHEMISTRY_OBJECT, "Temperature", Common::FloatToString(mTemperature));
  object->Place(CHEMISTRY_OBJECT, "Wattage", Common::FloatToString(mWattage));
  object->Place(CHEMISTRY_OBJECT, "Scale", Common::FloatToString(mScale));
  object->Place(CHEMISTRY_OBJECT, "Falloff", Common::FloatToString(mFalloff));
  object->Place(CHEMISTRY_OBJECT, "DirectionX", Common::FloatToString(mDirectionality.x));
  object->Place(CHEMISTRY_OBJECT, "DirectionY", Common::FloatToString(mDirectionality.y));
  object->Place(CHEMISTRY_OBJECT, "DirectionZ", Common::FloatToString(mDirectionality.z));
}

/**
 * @brief Deserialize from parser into memory.
 * @param aParser Parser to gather data from.
 */
void ChemistryElement::Deserialize(Parser& aParser)
{
  HashString const CHEMISTRY_OBJECT = "ChemistryElement";
  if(aParser.Find(CHEMISTRY_OBJECT, "Name"))
    mName = aParser.Find(CHEMISTRY_OBJECT, "Name")->GetValue();
  if(aParser.Find(CHEMISTRY_OBJECT, "Temperature"))
    mTemperature = aParser.Find(CHEMISTRY_OBJECT, "Temperature")->GetValue().ToFloat();
  if(aParser.Find(CHEMISTRY_OBJECT, "Wattage"))
    mWattage = aParser.Find(CHEMISTRY_OBJECT, "Wattage")->GetValue().ToFloat();
  if(aParser.Find(CHEMISTRY_OBJECT, "Scale"))
    mScale = aParser.Find(CHEMISTRY_OBJECT, "Scale")->GetValue().ToFloat();
  if(aParser.Find(CHEMISTRY_OBJECT, "Falloff"))
    mFalloff = aParser.Find(CHEMISTRY_OBJECT, "Falloff")->GetValue().ToFloat();
  if(aParser.Find(CHEMISTRY_OBJECT, "DirectionX"))
    mDirectionality.x = aParser.Find(CHEMISTRY_OBJECT, "DirectionX")->GetValue().ToFloat();
  if(aParser.Find(CHEMISTRY_OBJECT, "DirectionY"))
    mDirectionality.y = aParser.Find(CHEMISTRY_OBJECT, "DirectionY")->GetValue().ToFloat();
  if(aParser.Find(CHEMISTRY_OBJECT, "DirectionZ"))
    mDirectionality.z = aParser.Find(CHEMISTRY_OBJECT, "DirectionZ")->GetValue().ToFloat();
}

/**
 * @brief Serialize to LUA.
 */
void ChemistryElement::SerializeLUA()
{
  SLB::Class<ChemistryElement>("ChemistryElement");
}