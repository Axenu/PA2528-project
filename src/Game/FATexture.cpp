#include "Game/FATexture.h"

FATexture::FATexture() {

}

FATexture::FATexture(const GLchar *path) {

}

FATexture::FATexture(SharedPtr<Texture> texture) : _texture(texture) {
	// TODO: Actually use the texture.



}

FATexture::~FATexture() {

}

GLuint FATexture::createTexture(const GLchar *path) {

    // ILuint image = ilGenImage();
    // ilBindImage(image);
    // ILboolean loadSucsess = ilLoadImage(path);
    // if (!loadSucsess) {
    //     ilBindImage(0);
    //     std::cout << "File not found: '" << path << "'" << std::endl;
    //     ilDeleteImage(image);
    //     return 0;
    // }
    //
    // ILboolean convertSuccess = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    // if (!convertSuccess) {
    //     ilBindImage(0);
    //     std::cout << "Failed to conver image: '" << path << "'" << std::endl;
    //     ilDeleteImage(image);
    //     return 0;
    // }
	// glEnable(GL_TEXTURE_2D);
    // GLuint texture;
    // glGenTextures(1, &texture);
    //
    // glBindTexture(GL_TEXTURE_2D, texture);
    //
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //
	// glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
	// glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
    //
    // glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
    // glBindTexture(GL_TEXTURE_2D, 0);
    //
    // ilBindImage(0);
    // ilDeleteImage(image);
    return -1;
}

GLuint FATexture::getTexture(Texture *texture) {
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//std::cout << texture->width << std::endl;
	//std::cout << texture->height << std::endl;
	//std::cout << texture->composition << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->image);
	return tex;
}

GLuint FATexture::getDefaultTexture() {
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	float pixels[] = {
	    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
	    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
	return tex;
}
