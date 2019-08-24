#ifndef __JFramework__ChemistryMaterial__
#define __JFramework__ChemistryMaterial__

#include "Component.h"
#include "Common.h"
#include "ChemistryManager.h"

class ChemistryMaterial : public Component
{
private:
  ChemistryManager* mManager;
  HashString        mName;
  float             mCurrentTemperature;
  float             mCurrentWattage;
  float             mBoilingPoint;
  float             mMeltingPoint;
  float             mFreezingPoint;
  float             mConductivity;
  float             mHeatTransferRate;
  
  static HashType const  sUID;

public:
  ChemistryMaterial(ChemistryManager* aManager);
  ChemistryMaterial(ChemistryMaterial const &aChemistryMaterial);
  virtual ~ChemistryMaterial();
  
  // Getters
  ChemistryManager* GetManager() const;
  HashString const GetName() const;
  float const GetCurrentTemperature() const;
  float const GetCurrentWattage() const;
  float const GetBoilingPoint() const;
  float const GetMeltingPoint() const;
  float const GetFreezingPoint() const;
  float const GetConductivity() const;
  float const GetHeatTransferRate() const;
  
  // Setters
  void SetName(HashString const &aName);
  void SetCurrentTemperature(float const aCurrentTemperature);
  void SetCurrentWattage(float const aCurrentWattage);
  void SetBoilingPoint(float const aBoilingPoint);
  void SetMeltingPoint(float const aMeltingPoint);
  void SetFreezingPoint(float const aFreezingPoint);
  void SetConductivity(float const aConductivity);
  void SetHeatTransferRate(float const aHeatTransferRate);
  
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

#endif // __JFramework__ChemistryMaterial__
