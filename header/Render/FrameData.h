#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include "Game/Camera.h"

class FrameData {
public:
	FrameData();
	~FrameData();

	glm::mat4 VPMatrix;
	glm::mat4 VMatrix;
	glm::mat4 PMatrix;

	//Camera *_cam;
};