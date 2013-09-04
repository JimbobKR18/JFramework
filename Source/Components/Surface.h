#ifndef __JFramework_Surface_h_
#define __JFramework_Surface_h_

#include "Component.h"
#include "TextureCoordinates.h"

class GraphicsManager;

enum Viewspace
{
  VIEW_RELATIVE_TO_CAMERA = 0,
  VIEW_ABSOLUTE
};

class Surface : public Component
{
private:
  TextureCoordinates* mTexCoord;
  GraphicsManager*    mManager;
  Viewspace           mViewmode;
public:
	Surface();
	Surface(GraphicsManager *aManager);
	virtual ~Surface() {}

	// Getters
	GraphicsManager*    GetManager() const { return mManager; }
	Viewspace           GetViewMode() const { return mViewmode; }

	// Setters
	void                SetViewMode(Viewspace const &aViewmode) { mViewmode = aViewmode; }
  
  // Texture Coordinates
  TextureCoordinates* GetTextureData() const;
  void                SetTextureCoordinateData(int const aNumAnimations, std::vector<int> const aNumFrames);
  void                SetAnimated(bool aAnimated);
  
  // Batching
	virtual unsigned    GetTextureID() const = 0;
  
  // Derived from Component
	void                Update();
	void                SendMessage(Message const &aMessage);
	void                ReceiveMessage(Message const &aMessage);
  void                Serialize(Parser &aParser);
  void                Deserialize(Parser &aParser);
	static std::string  GetName() {return "Surface";}
};

#endif
