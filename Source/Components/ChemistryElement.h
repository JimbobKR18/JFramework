#ifndef __JFramework__ChemistryElement__
#define __JFramework__ChemistryElement__

#include "Component.h"
#include "Common.h"
#include "MathExt.h"
#include "ChemistryManager.h"

class ChemistryElement : public Component
{
private:
  ChemistryManager*     mManager;
  HashString            mName;
  float                 mTemperature;
  float                 mWattage;
  float                 mScale;
  float                 mFalloff;
  Vector3               mDirectionality;
  
  static int const  sUID;
public:
  ChemistryElement(ChemistryManager* aManager);
  virtual ~ChemistryElement();
  
  // Getters
  ChemistryManager* GetManager() const;
  HashString const GetName() const;
  float const GetTemperature() const;
  float const GetWattage() const;
  float const GetScale() const;
  float const GetFalloff() const;
  Vector3 const &GetDirectionality() const;
  
  // Setters
  void SetName(HashString const &aName);
  void SetTemperature(float const aTemperature);
  void SetWattage(float const aWattage);
  void SetScale(float const aScale);
  void SetFalloff(float const aFalloff);
  void SetDirectionality(Vector3 const &aDirectionality);

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

#endif // __JFramework__ChemistryElement__
