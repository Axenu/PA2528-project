#include "Render/FrameObject.h"



FrameObject::FrameObject()
{
	_head = nullptr;
	_tail = nullptr;
	_current = nullptr;
	_stack = new StackAllocator((1 << 15));
	_stack->setTrackingName("StackAllocator FrameObject");
}


FrameObject::~FrameObject()
{
	//this->clear();
	//delete _stack;
}

void FrameObject::addObject(FANode *node) {
	//objects.push_back(node);
	Node *n = _stack->alloc<Node>();
	//Node *n = new Node();
	if (n == nullptr) return;
	if (_head == nullptr) {
		_head = n;
		_current = n;
	}
	n->node = node;
	n->next = nullptr;
	n->prev = _tail;
	if (_tail != nullptr)
		_tail->next = n;
	_tail = n;
}

void FrameObject::setFrameData(FrameData &fd) {
	_fData = fd;
}

FANode * FrameObject::getNext()
{
	Node *n = _current;
	if (n == nullptr) return nullptr;
	_current = n->next;
	return n->node;
}

void FrameObject::clear() {
	//this->objects.clear();
	if (_head != nullptr) {
		_stack->dealloc(_head);

		//if default:
		/*Node *n = _head;
		Node *next = n->next;
		while (next != nullptr)
		{
			delete n;
			n = next;
			next = n->next;
		}*/
		_head = nullptr;
		_tail = nullptr;
		_current = nullptr;
	}
}