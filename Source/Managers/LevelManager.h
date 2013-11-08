#ifndef __JFramework_LevelManager_h_
#define __JFramework_LevelManager_h_

#include "Manager.h"
#include "Level.h"

class LevelManager : public Manager
{
private:
	Level*              mActiveLevel;
	std::vector<Level*> mLevels;
public:
	LevelManager(GameApp *aApp);
	~LevelManager();

	Level*              CreateLevel(std::string const &aFilename);
	Level*              GetLevel(std::string const &aLevelName);
	void                DeleteLevel(Level *aLevel);
	void                LoadLevelDelayed(std::string const &aLevelName, bool aReset = false);
	void                LoadLevel(std::string const &aLevelName, bool aReset = false);
  Level*              GetActiveLevel();
  void                SetActiveLevel(Level *aLevel);
  // Derived from Manager
	virtual void        Update();
	virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
	static std::string  GetName() {return "LevelManager";}
	static void         SerializeLUA();
	void                AddLevel(Level *aLevel);
	void                RemoveLevel(Level *aLevel);
public:
  typedef std::vector<Level*>::iterator LevelsIT;
};

#endif
