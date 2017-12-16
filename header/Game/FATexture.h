#ifndef __FireArrow__FATexture__
#define __FireArrow__FATexture__

//#define GLFW_INCLUDE_GLCOREARB

#include <iostream>
#include "gl_include.h"
//#include <GLFW/glfw3.h>
// #include <IL/il.h>

class FATexture {
private:
	// GLint textureID;

public:
	FATexture();
	FATexture(const GLchar *path);
	~FATexture();

	static GLuint createTexture(const GLchar *path);
    static GLuint getDefaultTexture();
};

#endif