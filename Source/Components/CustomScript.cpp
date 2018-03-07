#include "CustomScript.h"
#include "LUATypes.h"

int const CustomScript::sUID = Common::StringHashFunction("CustomScript");

CustomScript::CustomScript() : Component(CustomScript::sUID), mFileName(), mUpdateFunctionName()
{
}

CustomScript::~CustomScript()
{
}

/**
 * @brief Get lua file name.
 * @return Name of lua file.
 */
HashString const CustomScript::GetFileName() const
{
  return mFileName;
}

/**
 * @brief Get update function name in lua.
 * @return Function name in lua.
 */
HashString const CustomScript::GetUpdateFunctionName() const
{
  return mUpdateFunctionName;
}

/**
 * @brief Get value stored in CustomScript.
 * @param aFieldName Name of field to retrieve.
 * @return Value if found, else assert.
 */
HashString const CustomScript::GetValue(HashString const &aFieldName)
{
  if(mValues.find(aFieldName.ToHash()) == mValues.end())
  {
    DebugLogPrint("Could not find field of name %s in CustomScript.\n", aFieldName.ToCharArray());
    assert(!"Could not find field in CustomScript.");
  }
  
  return mValues[aFieldName.ToHash()];
}

/**
 * @brief Set lua file name.
 * @param aFileName Lua file name.
 */
void CustomScript::SetFileName(HashString const &aFileName)
{
  mFileName = aFileName;
}

/**
 * @brief Set lua update function name.
 * @param aUpdateFunctionName Lua update function name.
 */
void CustomScript::SetUpdateFunctionName(HashString const &aUpdateFunctionName)
{
  mUpdateFunctionName = aUpdateFunctionName;
}

/**
 * @brief Set field value.
 * @param aFieldName Name of field.
 * @param aFieldValue Value of field.
 */
void CustomScript::SetValue(HashString const &aFieldName, HashString const &aFieldValue)
{
  mValues[aFieldName.ToHash()] = aFieldValue;
}

/**
 * @brief Call lua script update function, does custom behavior.
 */
void CustomScript::Update()
{
  LUABind::LoadFunction(mFileName, mUpdateFunctionName, GetOwner());
}

/**
 * @brief Send message to owner.
 * @param aMessage Message to send.
 */
void CustomScript::SendMessage(Message const& aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

/**
 * @brief Receive message.
 * @param aMessage Message to receive.
 */
void CustomScript::ReceiveMessage(Message const& aMessage)
{
}

/**
 * @brief Serialize out to parser.
 * @param aNode ParserNode to write to.
 */
void CustomScript::Serialize(ParserNode *aNode)
{
  HashString const CUSTOMSCRIPT_OBJECT = "CustomScript";
  aNode->Place(CUSTOMSCRIPT_OBJECT, "");
  
  ParserNode *customScript = aNode->Find(CUSTOMSCRIPT_OBJECT);
  customScript->Place("FileName", mFileName);
  customScript->Place("UpdateFunctionName", mUpdateFunctionName);
}

/**
 * @brief Deserialize from parser.
 * @param aNode ParserNode to read from.
 */
void CustomScript::Deserialize(ParserNode *aNode)
{
  if(aNode->Find("FileName"))
    mFileName = aNode->Find("FileName")->GetValue();
  if(aNode->Find("UpdateFunctionName"))
    mUpdateFunctionName = aNode->Find("UpdateFunctionName")->GetValue();
}

/**
 * @brief Serialize to lua.
 */
void CustomScript::SerializeLUA()
{
  SLB::Class<CustomScript>("CustomScript")
    .inherits<Component>()
    .set("GetValue", &CustomScript::GetValue)
    .set("SetValue", &CustomScript::SetValue);
}
