#ifndef CUSTOMSCRIPT_H
#define CUSTOMSCRIPT_H

#include "Component.h" // Base class: Component

class CustomScript : public Component
{
private:
  HashString mFileName;
  HashString mUpdateFunctionName;
  
  static int const  sUID;
public:
  CustomScript();
  virtual ~CustomScript();
  
  // Getters
  HashString const GetFileName() const;
  HashString const GetUpdateFunctionName() const;
  
  // Setters
  void SetFileName(HashString const &aFileName);
  void SetUpdateFunctionName(HashString const &aUpdateFunctionName);

  // Virtuals derived from Component
  virtual void Update();
  virtual void SendMessage(Message const& aMessage);
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void Serialize(ParserNode *aNode);
  virtual void Deserialize(ParserNode *aNode);
  
  // Statics
  static void SerializeLUA();
  static int GetUID() {return sUID;}
};

#endif // CUSTOMSCRIPT_H
