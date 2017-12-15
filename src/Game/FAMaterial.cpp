#include <Game/FAMaterial.h>
#include "glm/gtx/string_cast.hpp"

FAMaterial::FAMaterial() {

	this->shader = new Shader("Basic");
	MVPLocation = glGetUniformLocation(this->shader->_shaderProgram, "mvp");
	_textureLocation = glGetUniformLocation(this->shader->_shaderProgram, "tex");
	if (MVPLocation == -1) {
		std::cout << "loading MVPLocation failed" << std::endl;
	}
	if (_textureLocation == -1) {
		std::cout << "loading _textureLocation failed" << std::endl;
	}
}

void FAMaterial::bind(FrameData &fData) {

	glm::mat4 MVPMatrix = fData.VPMatrix * modelMatrix;
	glUseProgram(shader->_shaderProgram);
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVPMatrix[0][0]);
	glUniform1i(_textureLocation, 0);
	if (_hasTexture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture);
		std::cout << _texture << std::endl;
	}

}

void FAMaterial::setModelMatrix(glm::mat4 &modelMatrix) {
	this->modelMatrix = modelMatrix;
}

void FAMaterial::setTexture(GLint texture) {
	if (texture < 0) {
		_hasTexture = false;
		// this->texture = texture;
	} else {
		_hasTexture = true;
		_texture = texture;
	}
}

FAMaterial::~FAMaterial() {

}
