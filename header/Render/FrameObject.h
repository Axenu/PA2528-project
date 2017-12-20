#pragma once

#include "node.h"
#include "Render/FrameData.h"
#include "PA2528-2/StackAllocator.h"

class FrameObject
{
	struct Node {
		FANode *node;
		Node *next;
		Node *prev;
	};
private:
	//change to a linked list?
	// store everything in a stack?
	Node *_head;
	Node *_tail;
	Node *_current;
	int _count;
	StackAllocator *_stack;

public:
	FrameObject();
	~FrameObject();

	void addObject(FANode *node);
	void setFrameData(FrameData &fd);
	FANode *getNext();
	//void addCamera(Camera *cam);

	void clear();

	//std::vector<FANode *> objects;
	FrameData _fData;
};

