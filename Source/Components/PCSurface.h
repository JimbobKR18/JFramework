#ifndef __JFramework_PCSurface_h_
#define __JFramework_PCSurface_h_

#if defined(_WIN32)
  #include <Windows.h>
  #include <gl/gl.h>
  #include "SDL.h"
  #undef SendMessage
#elif defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <OpenGL/glext.h>
  #include <SDL2/SDL.h>
  #include <SDL2_ttf/SDL_ttf.h>
#else
  #include <GL/gl.h>
  #include <GL/glext.h>
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_ttf.h>
#endif

#include "Surface.h"
#include "MathExt.h"

enum WrapMode
{
  REPEAT = 0,
  CLAMP
};

class PCSurface : public Surface
{
private:
	GLuint        mTextureID;
	SDL_Surface*  mSurface;
	GLenum        mTextureFormat;
	GLint         mNumberOfColors;
	GLint         mWrapMode;
	TTF_Font*     mFont;
  
  static int const sUID;

public:
	PCSurface();
  PCSurface(GraphicsManager *aManager);
	virtual ~PCSurface();

	void                LoadImage(std::string const &aName);
	Vector3             LoadText(std::string const &aFont, std::string const &aText, Vector4 const &aForegroundColor, Vector4 const &aBackgroundColor, int aSize, int aMaxWidth);
  // Batching
  unsigned            GetIndexValue() const;
  // Wrap Mode
  void                SetWrapMode(WrapMode const &aWrapMode);

	void                Update();
	void                SendMessage(Message const &aMessage);
	void                ReceiveMessage(Message const &aMessage);
  void                Serialize(Parser &aParser);
  void                Deserialize(Parser &aParser);
  static void         SerializeLUA();
	static int           GetUID() {return sUID;}

	unsigned            GetTextureID() const;
};

#endif
