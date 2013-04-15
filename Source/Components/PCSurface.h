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
  #include <SDL/SDL.h>
#else
  #include <GL/gl.h>
  #include <GL/glext.h>
  #include <SDL/SDL.h>
#endif

#include "Surface.h"

class PCSurface : public Surface
{
private:
	GLuint        mTextureID;
	SDL_Surface*  mSurface;
	GLenum        mTextureFormat;
	GLint         mNumberOfColors;

public:
	PCSurface();
  PCSurface(GraphicsManager *aManager);
	virtual ~PCSurface();

	void                LoadImage(std::string const &aName);
  // Batching
  unsigned            GetIndexValue();

	void                Update();
	void                SendMessage(Message const &aMessage);
	void                ReceiveMessage(Message const &aMessage);
  void                Serialize(Parser &aParser);
  void                Deserialize(Parser &aParser);
	static std::string  GetName() {return "Surface";}

	unsigned            GetTextureID() const;
};

#endif
