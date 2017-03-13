#include "ChemistryMaterial.h"
#include "ChemistryElement.h"
#include "LUATypes.h"
#include "CollisionMessage.h"

int const ChemistryMaterial::sUID = Common::StringHashFunction("ChemistryMaterial");

ChemistryMaterial::ChemistryMaterial(ChemistryManager* aManager) : Component(ChemistryMaterial::sUID), mManager(aManager), 
  mName(""), mCurrentTemperature(0), mCurrentWattage(0), mBoilingPoint(FLT_MAX), mMeltingPoint(0), 
  mFreezingPoint(FLT_MIN), mConductivity(0), mHeatTransferRate(1.0f)
{
}

ChemistryMaterial::~ChemistryMaterial()
{
  mManager->RemoveMaterial(this);
  mManager = nullptr;
}

/**
 * @return Manager
 */
ChemistryManager* ChemistryMaterial::GetManager() const
{
  return mManager;
}

/**
 * @return Name.
 */
HashString const ChemistryMaterial::GetName() const
{
  return mName;
}

/**
 * @return Current temperature.
 */
float const ChemistryMaterial::GetCurrentTemperature() const
{
  return mCurrentTemperature;
}

/**
 * @return Current wattage.
 */
float const ChemistryMaterial::GetCurrentWattage() const
{
  return mCurrentWattage;
}

/**
 * @return Boiling point.
 */
float const ChemistryMaterial::GetBoilingPoint() const
{
  return mBoilingPoint;
}

/**
 * @return Melting point.
 */
float const ChemistryMaterial::GetMeltingPoint() const
{
  return mMeltingPoint;
}

/**
 * @return Freezing point.
 */
float const ChemistryMaterial::GetFreezingPoint() const
{
  return mFreezingPoint;
}

/**
 * @return Conductivity.
 */
float const ChemistryMaterial::GetConductivity() const
{
  return mConductivity;
}

/**
 * @return Heat transfer rate per second.
 */
float const ChemistryMaterial::GetHeatTransferRate() const
{
  return mHeatTransferRate;
}

/**
 * @brief Set name of material.
 * @param aName Name.
 */
void ChemistryMaterial::SetName(HashString const &aName)
{
  mName = aName;
}

/**
 * @brief Set current temperature of material.
 * @param aCurrentTemperature Temperature.
 */
void ChemistryMaterial::SetCurrentTemperature(float const aCurrentTemperature)
{
  mCurrentTemperature = aCurrentTemperature;
}

/**
 * @brief Set current wattage of material.
 * @param aCurrentWattage Wattage.
 */
void ChemistryMaterial::SetCurrentWattage(float const aCurrentWattage)
{
  mCurrentWattage = aCurrentWattage;
}

/**
 * @brief Set boiling point of material.
 * @param aBoilingPoint Boiling point.
 */
void ChemistryMaterial::SetBoilingPoint(float const aBoilingPoint)
{
  mBoilingPoint = aBoilingPoint;
}

/**
 * @brief Set melting point of material.
 * @param aMeltingPoint Melting point.
 */
void ChemistryMaterial::SetMeltingPoint(float const aMeltingPoint)
{
  mMeltingPoint = aMeltingPoint;
}

/**
 * @brief Set freezing point of material.
 * @param aFreezingPoint Freezing point.
 */
void ChemistryMaterial::SetFreezingPoint(float const aFreezingPoint)
{
  mFreezingPoint = aFreezingPoint;
}

/**
 * @brief Set conductivity of material.
 * @param aConductivity Conductivity.
 */
void ChemistryMaterial::SetConductivity(float const aConductivity)
{
  mConductivity = aConductivity;
}

/**
 * @brief Set heat transfer rate of material.
 * @param aHeatTransferRate Heat transfer rate per second.
 */
void ChemistryMaterial::SetHeatTransferRate(float const aHeatTransferRate)
{
  mHeatTransferRate = aHeatTransferRate;
}

/**
 * @brief Update loop
 */
void ChemistryMaterial::Update()
{
}

/**
 * @brief Send message to upper levels.
 * @param aMessage Message to send.
 */
void ChemistryMaterial::SendMessage(Message const& aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

/**
 * @brief Receive message from above.
 * @param aMessage Message to receive.
 */
void ChemistryMaterial::ReceiveMessage(Message const& aMessage)
{
  if(aMessage.GetDescription() != "Collision")
    return;

  CollisionMessage *message = (CollisionMessage*)&aMessage;
  GameObject *otherBody = message->GetObject(0) != GetOwner() ?
                          message->GetObject(0) : message->GetObject(1);
  
  if(otherBody->HAS<ChemistryMaterial>())
  {
    ChemistryMaterial *otherChemical = otherBody->GET<ChemistryMaterial>();
    Vector3 direction = message->GetObject(0) != GetOwner() ? 
                        message->GetNormal() : message->GetNormal().Invert();
                        
    float otherTemperature = otherChemical->GetCurrentTemperature();
    float otherWattage = otherChemical->GetCurrentWattage();
    
    if(otherTemperature < mCurrentTemperature)
    {
      mCurrentTemperature -= mHeatTransferRate * mManager->GetOwningApp()->GetAppStep();
    }
    else if(otherTemperature > mCurrentTemperature)
    {
      mCurrentTemperature += mHeatTransferRate * mManager->GetOwningApp()->GetAppStep();
    }
    
    if(otherWattage > mCurrentWattage)
    {
      mCurrentWattage = otherWattage * mConductivity;
    }
    
    HashString luaFunctionName = mName + "_" + otherChemical->GetName() + "_Reaction";
    if(!LUABind::LoadFunction<ChemistryMaterial*>("ChemistryMessages.LUA",
                                     luaFunctionName,
                                     this, otherChemical))
    {
      DebugLogPrint("No chemistry function %s to call in LUA.\n", luaFunctionName.ToCharArray());
    }
  }
  else if(otherBody->HAS<ChemistryElement>())
  {
    ChemistryElement *otherChemical = otherBody->GET<ChemistryElement>();
    Vector3 direction = message->GetObject(0) != GetOwner() ? 
                        message->GetNormal() : message->GetNormal().Invert();
                        
    float otherTemperature = otherChemical->GetTemperature();
    float otherWattage = otherChemical->GetWattage();
    
    if(otherTemperature < mCurrentTemperature)
    {
      mCurrentTemperature -= mHeatTransferRate * mManager->GetOwningApp()->GetAppStep();
    }
    else if(otherTemperature > mCurrentTemperature)
    {
      mCurrentTemperature += mHeatTransferRate * mManager->GetOwningApp()->GetAppStep();
    }
    
    if(otherWattage > mCurrentWattage)
    {
      mCurrentWattage = otherWattage * mConductivity;
    }
    
    HashString luaFunctionName = mName + "_" + otherChemical->GetName() + "_Reaction";
    if(!LUABind::LoadFunction<ChemistryMaterial*>("ChemistryMessages.LUA",
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
void ChemistryMaterial::Serialize(Parser& aParser)
{
  HashString const objectName = HashString("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  HashString const CHEMISTRY_OBJECT = "ChemistryMaterial";
  Root* object = aParser.Find(objectName);
  
  object->Place(objectName, CHEMISTRY_OBJECT, "");
  object->Place(CHEMISTRY_OBJECT, "Name", mName);
  object->Place(CHEMISTRY_OBJECT, "BoilingPoint", Common::FloatToString(mBoilingPoint));
  object->Place(CHEMISTRY_OBJECT, "MeltingPoint", Common::FloatToString(mMeltingPoint));
  object->Place(CHEMISTRY_OBJECT, "FreezingPoint", Common::FloatToString(mFreezingPoint));
  object->Place(CHEMISTRY_OBJECT, "Conductivity", Common::FloatToString(mConductivity));
  object->Place(CHEMISTRY_OBJECT, "HeatTransferRate", Common::FloatToString(mHeatTransferRate));
  object->Place(CHEMISTRY_OBJECT, "StartingTemperature", Common::FloatToString(mCurrentTemperature));
  object->Place(CHEMISTRY_OBJECT, "StartingWattage", Common::FloatToString(mCurrentWattage));
}

/**
 * @brief Deserialize from parser into memory.
 * @param aParser Parser to gather data from.
 */
void ChemistryMaterial::Deserialize(Parser& aParser)
{
  HashString const CHEMISTRY_OBJECT = "ChemistryMaterial";
  mName = aParser.Find(CHEMISTRY_OBJECT, "Name")->GetValue();
  mBoilingPoint = aParser.Find(CHEMISTRY_OBJECT, "BoilingPoint")->GetValue().ToFloat();
  mMeltingPoint = aParser.Find(CHEMISTRY_OBJECT, "MeltingPoint")->GetValue().ToFloat();
  mFreezingPoint = aParser.Find(CHEMISTRY_OBJECT, "FreezingPoint")->GetValue().ToFloat();
  mConductivity = aParser.Find(CHEMISTRY_OBJECT, "Conductivity")->GetValue().ToFloat();
  mHeatTransferRate = aParser.Find(CHEMISTRY_OBJECT, "HeatTransferRate")->GetValue().ToFloat();
  
  // Optional
  if(aParser.Find(CHEMISTRY_OBJECT, "StartingTemperature"))
    mCurrentTemperature = aParser.Find(CHEMISTRY_OBJECT, "StartingTemperature")->GetValue().ToFloat();
  if(aParser.Find(CHEMISTRY_OBJECT, "StartingWattage"))
    mCurrentWattage = aParser.Find(CHEMISTRY_OBJECT, "StartingWattage")->GetValue().ToFloat();
}

/**
 * @brief Serialize to LUA.
 */
void ChemistryMaterial::SerializeLUA()
{
  SLB::Class<ChemistryMaterial>("ChemistryMaterial");
}