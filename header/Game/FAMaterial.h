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

private:
	bool _hasTexture;
	Shader *shader;
	glm::mat4 modelMatrix;


	GLint MVPLocation;
	GLint MLocation;
	GLint _textureLocation;

	GLint _texture;


public:
	FAMaterial();
	~FAMaterial();

	virtual void bind(FrameData &fData);
	virtual void setModelMatrix(glm::mat4 &modelMatrix);
	void setTexture(GLint texture);

protected:
	Camera *camera;
};

#endif
