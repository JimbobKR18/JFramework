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
 * @param aParser Parse to write to.
 */
void CustomScript::Serialize(Parser& aParser)
{
  HashString const objectName = HashString("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  HashString const CUSTOMSCRIPT_OBJECT = "CustomScript";
  ParserNode* object = aParser.Find(objectName);
  
  object->Place(objectName, CUSTOMSCRIPT_OBJECT, "");
  object->Place(CUSTOMSCRIPT_OBJECT, "Name", mFileName);
  object->Place(CUSTOMSCRIPT_OBJECT, "Temperature", mUpdateFunctionName);
}

/**
 * @brief Deserialize from parser.
 * @param aParser Parser to read from.
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
  SLB::Class<Surface>("CustomScript");
}
