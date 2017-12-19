#ifndef __First__FAMaterial__
#define __First__FAMaterial__

#define GLM_FORCE_RADIANS

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include "Camera.h"
#include "Render/Shader.h"
#include "Render/FrameData.h"

class FAMaterial {

protected:
	bool _hasTexture;
	Shader *shader;
	glm::mat4 modelMatrix;
	glm::vec4 _color;


	GLint MVPLocation;
	GLint MLocation;
	GLint _textureLocation;
	GLint _colorLocation;

	GLint _texture;

	Camera *camera;

public:
	FAMaterial();
	FAMaterial(GLint texture);
	~FAMaterial();

	virtual void bind(FrameData &fData);
	virtual void setModelMatrix(glm::mat4 &modelMatrix);
	virtual void setTexture(GLint texture);
	void setColor(glm::vec4 color);
	void setColorMemUsage(float mem_total, float mem_used);
};

#endif
