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

  // Derived from Manager
	virtual void        Update();
	virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
	static std::string  GetName() {return "ObjectManager";}

	GameObject*         CreateObject(std::string const &aFilename);
	void                ParseObject(GameObject *aObject);
	void                DeleteObject(GameObject *aObj);
	void                ClearObjects();
	void                AddObject(GameObject *aObj);
	void                RemoveObject(GameObject *aObj);
	static void         SerializeLUA();
private:
	void                ParseDictionary(GameObject *aObject, Parser &aParser);
  typedef std::vector<GameObject*>::iterator ObjectIT;
};

#endif
