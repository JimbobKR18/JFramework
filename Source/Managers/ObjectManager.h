#ifndef __JFramework_GameObject_H_
#define __JFramework_GameObject_H_

#include "GameObject.h"
#include "Manager.h"
#include "TextParser.h"

class ObjectManager : public Manager
{
private:
	std::vector<GameObject*> mObjects;
public:
	ObjectManager(GameApp* aApp);
	virtual ~ObjectManager();

	void Update();
	void SendMessage(Message const &aMessage);
	static std::string GetName() {return "ObjectManager";}

	GameObject *CreateObject(std::string const &aFilename);
	void DeleteObject(GameObject *aObj);
	void ClearObjects();
private:
	void AddObject(GameObject *aObj);
	void RemoveObject(GameObject *aObj);
	void ParseDictionary(GameObject *aObject, Parser &aParser);
};

#endif
