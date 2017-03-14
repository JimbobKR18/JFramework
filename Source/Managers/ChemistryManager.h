#ifndef __JFramework__ChemistryManager__
#define __JFramework__ChemistryManager__

#include "Manager.h" // Base class: Manager

class ChemistryMaterial;
class ChemistryElement;
class ChemicalFactory;

class ChemistryManager : public Manager
{
public:
  typedef std::vector<ChemistryMaterial*> MaterialContainer;
  typedef std::vector<ChemistryElement*> ElementContainer;
  typedef MaterialContainer::iterator MaterialIT;
  typedef ElementContainer::iterator ElementIT;
  typedef MaterialContainer::const_iterator MaterialConstIT;
  typedef ElementContainer::const_iterator ElementConstIT;
  
private:
  ChemicalFactory* mFactory;
  MaterialContainer mMaterials;
  ElementContainer mElements;
  float mCurrentTemperature;
  
  static unsigned const sUID;

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
  static unsigned     GetUID() {return sUID;}
};

#endif // __JFramework__ChemistryManager__
