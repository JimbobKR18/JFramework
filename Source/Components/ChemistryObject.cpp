#include "ChemistryObject.h"
#include "CollisionMessage.h"
#include "LUATypes.h"

int const ChemistryObject::sUID = Common::StringHashFunction("ChemistryObject");

ChemistryObject::ChemistryObject(HashString const &aName, ChemistryType const &aType) : Component(ChemistryObject::sUID),
  mName(aName), mType(aType), mProperties()
{
}

ChemistryObject::~ChemistryObject()
{
}

/**
 * @brief Update loop
 */
void ChemistryObject::Update()
{
  assert(!"This class should be derived from.");
}

/**
 * @brief Send message to upper levels.
 * @param aMessage Message to send.
 */
void ChemistryObject::SendMessage(Message const& aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

/**
 * @brief Receive message from above.
 * @param aMessage Message to receive.
 */
void ChemistryObject::ReceiveMessage(Message const& aMessage)
{
  if(aMessage.GetDescription() != "Collision")
    return;

  CollisionMessage *message = (CollisionMessage*)&aMessage;
  GameObject *otherBody = message->GetObject(0) != GetOwner() ?
                          message->GetObject(0) : message->GetObject(1);
  
  if(otherBody->HAS<ChemistryObject>())
  {
    ChemistryObject *otherChemical = otherBody->GET<ChemistryObject>();
    ChemistryType otherType = otherChemical->GetType();
    Vector3 direction = message->GetObject(0) != GetOwner() ? 
                        message->GetNormal() : message->GetNormal().Invert();
    
    if(mType == MATERIAL)
    {
      if(otherType == MATERIAL)
      {
        // Share chemical info
        for(ChemicalPropertyIT it = otherChemical->mProperties.begin(); it != otherChemical->mProperties.end(); ++it)
        {
          AddChemicalInformation((*it)->mName, (*it)->mDirection);
        }
      }
      else
      {
        // Take on element's chemical info
        AddChemicalInformation(otherChemical->GetName(), direction);
      }
    }
    else
    {
      if(otherType == ELEMENT)
      {
        // Take on element's chemical info
        AddChemicalInformation(otherChemical->GetName(), direction);
      }
    }
    
    HashString luaFunctionName = mName + "_" + otherChemical->GetName() + "_Reaction";
    if(!LUABind::LoadFunction<ChemistryObject*>("ChemistryMessages.LUA",
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
void ChemistryObject::Serialize(Parser& aParser)
{
  HashString const objectName = HashString("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  HashString const CHEMISTRY_OBJECT = "ChemistryObject";
  Root* object = aParser.Find(objectName);
  
  object->Place(objectName, CHEMISTRY_OBJECT, "");
  object->Place(CHEMISTRY_OBJECT, "Name", mName);
  
  switch(mType)
  {
  case MATERIAL:
    object->Place(CHEMISTRY_OBJECT, "Type", "MATERIAL");
    break;
  case ELEMENT:
    object->Place(CHEMISTRY_OBJECT, "Type", "ELEMENT");
    break;
  default:
    break;
  }
}

/**
 * @brief Deserialize from parser into memory.
 * @param aParser Parser to gather data from.
 */
void ChemistryObject::Deserialize(Parser& aParser)
{
  HashString const CHEMISTRY_OBJECT = "ChemistryObject";
  mName = aParser.Find(CHEMISTRY_OBJECT, "Name")->GetValue();
  
  HashString type = aParser.Find(CHEMISTRY_OBJECT, "Type")->GetValue();
  if(type == "MATERIAL")
  {
    mType = ChemistryType::MATERIAL;
  }
  else if(type == "ELEMENT")
  {
    mType = ChemistryType::ELEMENT;
  }
  else
  {
    assert(!"Incorrect chemistry type assigned.");
  }
}

/**
 * @brief Serialize to LUA.
 */
void ChemistryObject::SerializeLUA()
{
  SLB::Class<ChemistryObject>("ChemistryObject")
    .set("GetName", &ChemistryObject::GetName)
    .set("GetType", &ChemistryObject::GetType);
}

/**
 * @brief Add chemical information.
 * @param aName Name of chemical.
 * @param aDirection Direction of influence.
 */
void ChemistryObject::AddChemicalInformation(HashString const &aName, Vector3 const &aDirection)
{
  ChemicalInformation *newInfo = new ChemicalInformation(aName, aDirection);
  mProperties.push_back(newInfo);
}
