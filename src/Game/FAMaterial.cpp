#include <Game/FAMaterial.h>
#include "glm/gtx/string_cast.hpp"

FAMaterial::FAMaterial() {

	this->shader = new Shader("Basic");
	MVPLocation = glGetUniformLocation(this->shader->_shaderProgram, "mvp");
	if (MVPLocation == -1) {
		std::cout << "loading MVPLocation failed" << std::endl;
	}

	_hasTexture = false;
}
FAMaterial::FAMaterial(GLint texture) {

	this->shader = new Shader("BasicTexture");
	MVPLocation = glGetUniformLocation(this->shader->_shaderProgram, "mvp");
	_textureLocation = glGetUniformLocation(this->shader->_shaderProgram, "tex");
	if (MVPLocation == -1) {
		std::cout << "loading MVPLocation failed" << std::endl;
	}
	if (_textureLocation == -1) {
		std::cout << "loading _textureLocation failed" << std::endl;
	}

	_texture = texture;
	_hasTexture = true;
}

void FAMaterial::bind(FrameData &fData) {

	glm::mat4 MVPMatrix = fData.VPMatrix * modelMatrix;
	glUseProgram(shader->_shaderProgram);
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVPMatrix[0][0]);
	if (_hasTexture) {
		glUniform1i(_textureLocation, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture);
	}

}

void FAMaterial::setModelMatrix(glm::mat4 &modelMatrix) {
	this->modelMatrix = modelMatrix;
}

void FAMaterial::setTexture(GLint texture) {
	if (texture < 0) {
		_hasTexture = false;
		delete this->shader;
		this->shader = new Shader("Basic");
		MVPLocation = glGetUniformLocation(this->shader->_shaderProgram, "mvp");
		if (MVPLocation == -1) {
			std::cout << "loading MVPLocation failed" << std::endl;
		}
	} else {
		delete this->shader;
		this->shader = new Shader("BasicTexture");
		MVPLocation = glGetUniformLocation(this->shader->_shaderProgram, "mvp");
		_textureLocation = glGetUniformLocation(this->shader->_shaderProgram, "tex");
		if (MVPLocation == -1) {
			std::cout << "loading MVPLocation failed" << std::endl;
		}
		if (_textureLocation == -1) {
			std::cout << "loading _textureLocation failed" << std::endl;
		}

		_texture = texture;
		_hasTexture = true;
	}
}

FAMaterial::~FAMaterial() {

}
