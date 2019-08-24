#ifndef __JFramework_LevelManager_h_
#define __JFramework_LevelManager_h_

#include "Manager.h"
#include "Level.h"

class LevelManager : public Manager
{
public:
  typedef std::set<Level*> LevelContainer;
  typedef LevelContainer::iterator LevelIT;
  typedef LevelContainer::const_iterator LevelConstIT;
  
private:
  Level*         mActiveLevel;
  LevelContainer mLevels;

  static unsigned const sUID;
  
public:
  LevelManager(GameApp *aApp);
  ~LevelManager();

  Level*              CreateLevel(HashString const &aFilename, HashString const &aFolderName);
  Level*              GetLevel(HashString const &aLevelName);
  void                DeleteLevel(Level *aLevel);
  void                LoadLevelDelayed(HashString const &aLevelName, bool aReset = false);
  void                LoadLevel(HashString const &aLevelName, bool aReset = false);
  Level*              GetActiveLevel();
  // Derived from Manager
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "LevelManager";}
  static HashType GetUID() {return sUID;}
  static void         SerializeLUA();
  void                AddLevel(Level *aLevel);
  void                RemoveLevel(Level *aLevel);
};

#endif
