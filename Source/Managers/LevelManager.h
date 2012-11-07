#ifndef __JFramework_LevelManager_h_
#define __JFramework_LevelManager_h_

#include "Manager.h"
#include "Level.h"

class LevelManager : public Manager
{
private:
	std::vector<Level*> mLevels;
public:
	LevelManager();
	~LevelManager();

	Level *CreateLevel(std::string const &aFilename);
	void DeleteLevel(Level *aLevel);

	void Update();
	void SendMessage(Message const &aMessage);
	static std::string GetName() {return "LevelManager";}
private:
	void AddLevel(Level *aLevel);
	void RemoveLevel(Level *aLevel);
};

#endif
