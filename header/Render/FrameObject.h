#pragma once

#include "node.h"
#include "Render/FrameData.h"

class FrameObject
{

private:
	//change to a linked list?
	// store everything in a stack?
public:
	FrameObject(FrameData &fData);
	~FrameObject();

	void addObject(FANode *node);
	//void addCamera(Camera *cam);

	void clear();

	std::vector<FANode *> objects;
	FrameData fData;
};

