#include <Game/FAMaterial.h>
#include "glm/gtx/string_cast.hpp"

FAMaterial::FAMaterial() {

	this->shader = new Shader("Basic");
	MVPLocation = glGetUniformLocation(this->shader->_shaderProgram, "mvp");
	_colorLocation = glGetUniformLocation(this->shader->_shaderProgram, "color");
	if (MVPLocation == -1) {
		std::cout << "loading MVPLocation failed" << std::endl;
	}
	if (_colorLocation == -1) {
		std::cout << "loading colorLocation failed" << std::endl;
	}
	_hasTexture = false;
	_color = glm::vec4(1, 0, 0, 1);
}
FAMaterial::FAMaterial(GLint texture) {

	this->shader = new Shader("BasicTexture");
	MVPLocation = glGetUniformLocation(this->shader->_shaderProgram, "mvp");
	_colorLocation = glGetUniformLocation(this->shader->_shaderProgram, "color");
	_textureLocation = glGetUniformLocation(this->shader->_shaderProgram, "tex");
	if (MVPLocation == -1) {
		std::cout << "loading MVPLocation failed" << std::endl;
	}
	if (_textureLocation == -1) {
		std::cout << "loading _textureLocation failed" << std::endl;
	}
	if (_colorLocation == -1) {
		std::cout << "loading colorLocation failed" << std::endl;
	}
	_color = glm::vec4(1, 1, 0, 1);
	_texture = texture;
	_hasTexture = true;
}

void FAMaterial::bind(FrameData &fData) {

	glm::mat4 MVPMatrix = fData.VPMatrix * modelMatrix;
	glUseProgram(shader->_shaderProgram);
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVPMatrix[0][0]);
	glUniform4fv(_colorLocation, 1, &_color[0]);
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
		_colorLocation = glGetUniformLocation(this->shader->_shaderProgram, "color");
		if (MVPLocation == -1) {
			std::cout << "loading MVPLocation failed" << std::endl;
		}
		if (_colorLocation == -1) {
			std::cout << "loading colorLocation failed" << std::endl;
		}
	}
	else {
		delete this->shader;
		this->shader = new Shader("BasicTexture");
		MVPLocation = glGetUniformLocation(this->shader->_shaderProgram, "mvp");
		_colorLocation = glGetUniformLocation(this->shader->_shaderProgram, "color");
		_textureLocation = glGetUniformLocation(this->shader->_shaderProgram, "tex");
		if (MVPLocation == -1) {
			std::cout << "loading MVPLocation failed" << std::endl;
		}
		if (_textureLocation == -1) {
			std::cout << "loading _textureLocation failed" << std::endl;
		}
		if (_colorLocation == -1) {
			std::cout << "loading colorLocation failed" << std::endl;
		}

		_texture = texture;
		_hasTexture = true;
	}
}

void FAMaterial::setColor(glm::vec4 color) {
	_color = color;
}

void FAMaterial::setColorMemUsage(float mem_total, float mem_used)
{
	// calc red
	float red = 1.0f;
	if (mem_used < mem_total)
		red *= ((mem_total - mem_used) / mem_total);
	_color[0] = red;

	// green
	_color[1] = 1.0f - red;

	// blue
	_color[2] = 0.0f;

	// alpha
	_color[3] = 1.0f;
}

FAMaterial::~FAMaterial() {

}
