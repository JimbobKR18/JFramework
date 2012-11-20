#ifndef __JFramework_LevelManager_h_
#define __JFramework_LevelManager_h_

#include "Manager.h"
#include "Level.h"

class LevelManager : public Manager
{
private:
	Level *mActiveLevel;
	std::vector<Level*> mLevels;
public:
	LevelManager(GameApp *aApp);
	~LevelManager();

	Level *CreateLevel(std::string const &aFilename);
	Level *GetLevel(std::string const &aLevelName);
	void DeleteLevel(Level *aLevel);
	void LoadLevel(std::string const &aLevelName);

	void Update();
	void SendMessage(Message const &aMessage);
	static std::string GetName() {return "LevelManager";}
private:
	void AddLevel(Level *aLevel);
	void RemoveLevel(Level *aLevel);
};

#endif
