#ifndef __JFramework_Surface_h_
#define __JFramework_Surface_h_

#include "Component.h"
#include "TextureCoordinates.h"
#include "MathExt.h"

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
  Vector4             mColor;
  std::string         mFileName;
  bool                mNoRender;
public:
	Surface();
	Surface(GraphicsManager *aManager);
	virtual ~Surface();

	// Getters
	GraphicsManager*    GetManager() const { return mManager; }
	Viewspace           GetViewMode() const { return mViewmode; }
	Vector4&            GetColor() { return mColor; }
	std::string         GetFileName() { return mFileName; }

	// Setters
	void                SetViewMode(Viewspace const& aViewmode) { mViewmode = aViewmode; }
	void                SetColor(Vector4 const& aColor) { mColor = aColor; }
	void                SetFileName(std::string const& aFileName) { mFileName = aFileName; }
  
  // Texture Coordinates
  TextureCoordinates* GetTextureData() const;
  void                SetTextureCoordinateData(int const aNumAnimations, std::vector<int> const aNumFrames);
  void                SetAnimated(bool aAnimated);
  void                SetAnimation(int aAnimation);
  void                SetCurrentFrame(int aFrame);
  bool                CurrentAnimationCompleted();
  
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
