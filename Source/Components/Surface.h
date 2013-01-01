#ifndef __JFramework_Surface_h_
#define __JFramework_Surface_h_

#include "Component.h"
#include "TextureCoordinates.h"

class Surface : public Component
{
private:
  TextureCoordinates *mTexCoord;
public:
	Surface();
	virtual ~Surface() {}
  
  // Texture Coordinates
  TextureCoordinates *GetTextureData() const;
  void SetTextureCoordinateData(int const aNumAnimations, std::vector<int> const aNumFrames);
  void SetAnimated(bool aAnimated);

  // Derived from Component
	void Update();
	void SendMessage(Message const &aMessage);
	void ReceiveMessage(Message const &aMessage);
	static std::string GetName() {return "Surface";}
};

#endif
