#ifndef CUSTOMSCRIPT_H
#define CUSTOMSCRIPT_H

#include "Component.h" // Base class: Component

class CustomScript : public Component
{
private:
  HashString mFileName;
  HashString mUpdateFunctionName;
  std::unordered_map<HashType, HashString> mValues;
  
  static HashType const sUID;
public:
  CustomScript();
  CustomScript(CustomScript const &aCustomScript);
  virtual ~CustomScript();
  
  // Getters
  HashString const& GetFileName() const;
  HashString const& GetUpdateFunctionName() const;
  HashString const& GetValue(HashString const &aFieldName);
  
  // Setters
  void SetFileName(HashString const &aFileName);
  void SetUpdateFunctionName(HashString const &aUpdateFunctionName);
  void SetValue(HashString const &aFieldName, HashString const &aFieldValue);

  // Virtuals derived from Component
  virtual void Update();
  virtual void SendMessage(Message const& aMessage);
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void Serialize(ParserNode *aNode);
  virtual void Deserialize(ParserNode *aNode);
  virtual Component* Clone(GameObject *aNewOwner) const;
  
  // Statics
  static void SerializeLUA();
  static HashType GetUID() {return sUID;}
};

#endif // CUSTOMSCRIPT_H
