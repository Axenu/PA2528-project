#include "Render/FrameObject.h"



FrameObject::FrameObject()
{
	_head = nullptr;
	_tail = nullptr;
	_current = nullptr;
	_stack = new StackAllocator((1 << 10));
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
	_count += 1;
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
	//std::cout << "count: " << _count << std::endl;
	//this->objects.clear();
	_count = 0;
	if (_head != nullptr) {
		//_stack->dealloc(_head);

		//if default:
		Node *n = _tail;
		Node *prev;
		while (n != nullptr)
		{
			prev = n->prev;
			_stack->dealloc(n);
			n = prev;
		}
		_head = nullptr;
		_tail = nullptr;
		_current = nullptr;
	}
}