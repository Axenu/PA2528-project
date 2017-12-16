#pragma once

#include <iostream>
#include <string>
#include "gl/glInclude.h"
#include <IL/il.h>

class gl_Texture {
private:
	GLint _texture;

public:
	gl_Texture();
	gl_Texture(std::string path);
	~gl_Texture();

    void bindTexture(int index);

	static GLuint createTexture(std::string path);
};
