#include "Game/FAMaterialColor.h"


FAMaterialColor::FAMaterialColor()
{
	//this->shader = new Shader("Basic");
	this->shader = new Shader("ColorMemory");
	MVPLocation = glGetUniformLocation(this->shader->_shaderProgram, "mvp");
	MemoryColor = glGetUniformLocation(this->shader->_shaderProgram, "color");
	if (MVPLocation == -1) {
		std::cout << "loading MVPLocation failed" << std::endl;
	}
	if (MemoryColor == -1) {
		std::cout << "loading MemoryColor failed" << std::endl;
	}

	// default color
	m_color[0] = 0.0f;
	m_color[1] = 0.0f;
	m_color[2] = 0.0f;
	m_color[3] = 1.0f;
	//

	_hasTexture = false;
}

FAMaterialColor::FAMaterialColor(float color[4])
{
	//this->shader = new Shader("Basic");
	this->shader = new Shader("ColorMemory");
	MVPLocation = glGetUniformLocation(this->shader->_shaderProgram, "mvp");
	MemoryColor = glGetUniformLocation(this->shader->_shaderProgram, "color");
	if (MVPLocation == -1) {
		std::cout << "loading MVPLocation failed" << std::endl;
	}
	if (MemoryColor == -1) {
		std::cout << "loading MemoryColor failed" << std::endl;
	}

	// set color
	setColor(color);

	_hasTexture = false;
}

FAMaterialColor::~FAMaterialColor()
{
}

void FAMaterialColor::bind(FrameData &fData) {
	glm::mat4 MVPMatrix = fData.VPMatrix * modelMatrix;
	glUseProgram(shader->_shaderProgram);
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVPMatrix[0][0]);
	glUniform4fv(MemoryColor, 1, m_color);
	if (_hasTexture) {
		glUniform1i(_textureLocation, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture);
	}
}

void FAMaterialColor::setTexture(GLint texture){
	if (texture < 0) {
		_hasTexture = false;
		delete this->shader;
		this->shader = new Shader("ColorMemory");
		MVPLocation = glGetUniformLocation(this->shader->_shaderProgram, "mvp");
		MemoryColor = glGetUniformLocation(this->shader->_shaderProgram, "color");
		if (MVPLocation == -1) {
			std::cout << "loading MVPLocation failed" << std::endl;
		}
		if (MemoryColor == -1) {
			std::cout << "loading MemoryColor failed" << std::endl;
		}
	}
	else {
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

void FAMaterialColor::setColor(float color[4])
{
	m_color[0] = color[0];
	m_color[1] = color[1];
	m_color[2] = color[2];
	m_color[3] = color[3];
}

void FAMaterialColor::setColorMemFrag(float mem_total, float mem_used)
{
	// calc red
	float red = 1.0f; 
	if (mem_used < mem_total)
		red *= (mem_used / mem_total);
	m_color[0] = red;
	
	 // green
	m_color[1] = 1.0f - red;

	// blue
	m_color[2] = 0.0f;

	// alpha
	m_color[3] = 1.0f;
}