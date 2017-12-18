#ifndef __FireArrow__FATexture__
#define __FireArrow__FATexture__

//#define GLFW_INCLUDE_GLCOREARB

#include <iostream>
#include "gl_include.h"
#include "PA2528-3/SharedPtr.hpp"
#include "PA2528-3/Texture.hpp"

//#include <GLFW/glfw3.h>
// #include <IL/il.h>

class FATexture {
private:
	// GLint textureID;

public:
	FATexture();
	FATexture(const GLchar *path);
	FATexture(SharedPtr<Texture> texture);
	~FATexture();

	static GLuint createTexture(const GLchar *path);
    static GLuint getDefaultTexture();
	static GLuint getTexture(Texture *texture);

private:
	SharedPtr<Texture> _texture;
};

#endif
