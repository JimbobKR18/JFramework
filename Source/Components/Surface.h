#ifndef __JFramework_Surface_h_
#define __JFramework_Surface_h_

#include "Component.h"
#include "TextureCoordinates.h"

class GraphicsManager;

class Surface : public Component
{
private:
  TextureCoordinates* mTexCoord;
  GraphicsManager*    mManager;
public:
	Surface();
	Surface(GraphicsManager *aManager);
	virtual ~Surface() {}
  
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
	static std::string  GetName() {return "Surface";}
};

#endif
