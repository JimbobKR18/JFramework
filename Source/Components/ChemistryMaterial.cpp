#include "ChemistryMaterial.h"
#include "ChemistryElement.h"
#include "LUATypes.h"
#include "CollisionMessage.h"

HashType const ChemistryMaterial::sUID = Common::StringHashFunction("ChemistryMaterial");

ChemistryMaterial::ChemistryMaterial(ChemistryManager* aManager) : Component(ChemistryMaterial::sUID), mManager(aManager), 
  mName(""), mCurrentTemperature(0), mCurrentWattage(0), mBoilingPoint(FLT_MAX), mMeltingPoint(0), 
  mFreezingPoint(FLT_MIN), mConductivity(0), mHeatTransferRate(1.0f)
{
}

ChemistryMaterial::ChemistryMaterial(ChemistryMaterial const &aChemistryMaterial) : Component(ChemistryMaterial::sUID),
  mManager(aChemistryMaterial.mManager), mName(aChemistryMaterial.mName), mCurrentTemperature(aChemistryMaterial.mCurrentTemperature),
  mCurrentWattage(aChemistryMaterial.mCurrentWattage), mBoilingPoint(aChemistryMaterial.mBoilingPoint), mMeltingPoint(aChemistryMaterial.mMeltingPoint),
  mFreezingPoint(aChemistryMaterial.mFreezingPoint), mConductivity(aChemistryMaterial.mConductivity), mHeatTransferRate(aChemistryMaterial.mHeatTransferRate)
{
}

ChemistryMaterial::~ChemistryMaterial()
{
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
HashString const& ChemistryMaterial::GetName() const
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
  if(aMessage.GetDescription() != COLLISION)
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
    if(!LUABind::LoadFunction<ChemistryMaterial*>("ChemistryMessages.lua",
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
    if(!LUABind::LoadFunction<ChemistryMaterial*>("ChemistryMessages.lua",
                                     luaFunctionName,
                                     this, otherChemical))
    {
      DebugLogPrint("No chemistry function %s to call in LUA.\n", luaFunctionName.ToCharArray());
    }
  }
}

/**
 * @brief Serialize out.
 * @param aNode ParserNode to write to.
 */
void ChemistryMaterial::Serialize(ParserNode *aNode)
{
  HashString const CHEMISTRY_OBJECT = "ChemistryMaterial";
  
  aNode->Place(CHEMISTRY_OBJECT, "");
  ParserNode* object = aNode->Find(CHEMISTRY_OBJECT);
  
  object->Place("Name", mName);
  object->Place("BoilingPoint", Common::FloatToString(mBoilingPoint));
  object->Place("MeltingPoint", Common::FloatToString(mMeltingPoint));
  object->Place("FreezingPoint", Common::FloatToString(mFreezingPoint));
  object->Place("Conductivity", Common::FloatToString(mConductivity));
  object->Place("HeatTransferRate", Common::FloatToString(mHeatTransferRate));
  object->Place("StartingTemperature", Common::FloatToString(mCurrentTemperature));
  object->Place("StartingWattage", Common::FloatToString(mCurrentWattage));
}

/**
 * @brief Deserialize from parser into memory.
 * @param aNode ParserNode to read from.
 */
void ChemistryMaterial::Deserialize(ParserNode *aNode)
{
  if(aNode->Find("Name"))
    mName = aNode->Find("Name")->GetValue();
  if(aNode->Find("BoilingPoint"))
    mBoilingPoint = aNode->Find("BoilingPoint")->GetValue().ToFloat();
  if(aNode->Find("MeltingPoint"))
    mMeltingPoint = aNode->Find("MeltingPoint")->GetValue().ToFloat();
  if(aNode->Find("FreezingPoint"))
    mFreezingPoint = aNode->Find("FreezingPoint")->GetValue().ToFloat();
  if(aNode->Find("Conductivity"))
    mConductivity = aNode->Find("Conductivity")->GetValue().ToFloat();
  if(aNode->Find("HeatTransferRate"))
    mHeatTransferRate = aNode->Find("HeatTransferRate")->GetValue().ToFloat();
  if(aNode->Find("StartingTemperature"))
    mCurrentTemperature = aNode->Find("StartingTemperature")->GetValue().ToFloat();
  if(aNode->Find("StartingWattage"))
    mCurrentWattage = aNode->Find("StartingWattage")->GetValue().ToFloat();
}

/**
 * @brief Clone ChemistryMaterial
 * @param aNewOwner The new owner
 * @return Cloned ChemistryMaterial
 */
Component* ChemistryMaterial::Clone(GameObject *aNewOwner) const
{
  return new ChemistryMaterial(*this);
}

/**
 * @brief Serialize to LUA.
 */
void ChemistryMaterial::SerializeLUA()
{
  SLB::Class<ChemistryMaterial>("ChemistryMaterial")
    .inherits<Component>();
}
