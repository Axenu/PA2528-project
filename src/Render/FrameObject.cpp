#include "Render/FrameObject.h"



FrameObject::FrameObject(FrameData &fData)
{
	this->fData = fData;
}


FrameObject::~FrameObject()
{
}

void FrameObject::addObject(FANode *node) {
	objects.push_back(node);
}

void FrameObject::clear() {
	this->objects.clear();
}