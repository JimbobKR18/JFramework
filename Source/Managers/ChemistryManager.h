#ifndef __JFramework__ChemistryManager__
#define __JFramework__ChemistryManager__

#include "Manager.h" // Base class: Manager

class ChemistryMaterial;
class ChemistryElement;
class ChemicalFactory;

class ChemistryManager : public Manager
{
public:
  typedef std::unordered_set<ChemistryMaterial*> MaterialContainer;
  typedef std::unordered_set<ChemistryElement*> ElementContainer;
  typedef MaterialContainer::iterator MaterialIT;
  typedef ElementContainer::iterator ElementIT;
  typedef MaterialContainer::const_iterator MaterialConstIT;
  typedef ElementContainer::const_iterator ElementConstIT;
  
private:
  ChemicalFactory* mFactory;
  MaterialContainer mMaterials;
  ElementContainer mElements;
  float mCurrentTemperature;
  
  static HashType const sUID;

public:
  ChemistryManager(GameApp *aApp, float const aCurrentTemperature);
  virtual ~ChemistryManager();
  
  // Setters
  void                          SetChemicalFactory(ChemicalFactory *aFactory);
  
  // Object Management
  ChemistryMaterial*            CreateMaterial(HashString const &aName);
  void                          DeleteMaterial(ChemistryMaterial *aObject);
  void                          AddMaterial(ChemistryMaterial *aObject);
  void                          RemoveMaterial(ChemistryMaterial *aObject);
  void                          ClearMaterials();
  
  ChemistryElement*             CreateElement(HashString const &aName);
  void                          DeleteElement(ChemistryElement *aObject);
  void                          AddElement(ChemistryElement *aObject);
  void                          RemoveElement(ChemistryElement *aObject);
  void                          ClearElements();

  virtual void Update();
  virtual void ProcessDelayedMessage(Message* aMessage);
  virtual void SendMessage(Message const& aMessage);
  
  static std::string  GetName() {return "ChemistryManager";}
  static HashType     GetUID() {return sUID;}
};

#endif // __JFramework__ChemistryManager__
