#ifndef __JFramework_GraphicsManager_H_
#define __JFramework_GraphicsManager_H_

#include "Manager.h"
#include "Surface.h"
#include "Screen.h"

class GraphicsManager : public Manager
{
private:
	std::vector<Surface *> mSurfaces;
	Screen *mScreen;
public:
	GraphicsManager(GameApp *aApp);
	~GraphicsManager();

	void Update();
	void SendMessage(Message const &aMessage);
	static std::string GetName() {return "GraphicsManager";}

	Surface *CreateSurface();
	void DeleteSurface(Surface *aSurface);
	void ClearSurfaces();
private:
	void AddSurface(Surface *aSurface);
	void RemoveSurface(Surface *aSurface);
};

#endif
