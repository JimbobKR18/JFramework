#ifndef __JFramework__ChemistryObject__
#define __JFramework__ChemistryObject__

#include "Component.h"
#include "MathExt.h"

class ChemistryObject : public Component
{
public:
  enum ChemistryType
  {
    MATERIAL,
    ELEMENT
  };
  
private:
  // Helper struct to store information.
  struct ChemicalInformation
  {
    HashString  mName;
    Vector3     mDirection;
    
    ChemicalInformation(HashString const &aName, Vector3 const &aDirection) : 
      mName(aName), mDirection(aDirection) {}
    virtual ~ChemicalInformation() {}
  };
  
  typedef std::vector<ChemicalInformation*> ChemicalPropertyContainer;
  typedef ChemicalPropertyContainer::iterator ChemicalPropertyIT;
  typedef ChemicalPropertyContainer::const_iterator ChemicalPropertyConstIT;

  HashString                mName;
  ChemistryType             mType;
  ChemicalPropertyContainer mProperties;
  
  static int const          sUID;
  
public:
  ChemistryObject(HashString const &aName, ChemistryType const &aType);
  virtual ~ChemistryObject();
  
  // Getters
  HashString const GetName() const;
  ChemistryType const GetType() const;

  // Virtuals derived from Component
  virtual void Update();
  virtual void SendMessage(Message const& aMessage);
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void Serialize(Parser& aParser);
  virtual void Deserialize(Parser& aParser);
  
  // Statics
  static void SerializeLUA();
  static int GetUID() {return sUID;}
  
private:
  void AddChemicalInformation(HashString const &aName, Vector3 const &aDirection);
};

#endif // __JFramework__ChemistryObject__
