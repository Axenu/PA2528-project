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

class FAMaterial {

private:
	// std::vector<FAMaterialComponent*> components;
	Shader *shader;
	glm::mat4 viewProjectionMatrix;
	glm::mat4 modelMatrix;


	GLint MVPLocation;
	GLint MLocation;


public:
	FAMaterial();
	~FAMaterial();

	void setCamera(Camera *camera);

	virtual void bind();
	virtual void setViewProjectionwMatrix(glm::mat4 &VPMatrix);
	virtual void setModelMatrix(glm::mat4 &modelMatrix);

protected:
	Camera *camera;
};

#endif
