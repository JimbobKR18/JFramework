#ifndef __JFramework_PCSurface_h_
#define __JFramework_PCSurface_h_

#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL/SDL.h>

#include "Surface.h"

class PCSurface : public Surface
{
private:
	GLuint mTextureID;
	SDL_Surface *mSurface;
	GLenum mTextureFormat;
	GLint  mNumberOfColors;

public:
	PCSurface();
	~PCSurface();

	void LoadImage(std::string const &aName);

	void Update();
	void SendMessage(Message const &aMessage);
	void ReceiveMessage(Message const &aMessage);
	static std::string GetName() {return "Surface";}
};

#endif
