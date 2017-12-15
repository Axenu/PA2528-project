#include "Node.h"
#include <iostream>
#define GLM_FORCE_RADIANS
#include<glm/gtx/euler_angles.hpp>
#include<glm/mat3x3.hpp>


FANode::FANode()
	: _isActive(true), _position(0.f), _forward(0.f, 0.f, 1.f), _up(0.f, 1.f, 0.f), _scale(1.0f)
{
}
FANode::FANode(const glm::vec3 &position, FANode *parent)
	: _isActive(true), _position(position), _forward(0.f, 0.f, 1.f), _up(0.f, 1.f, 0.f), _scale(1.0f), _modelMatrix()
{
	setParent(parent);
}
FANode::~FANode() {

}

void FANode::calcModelMatrix()
{
	glm::mat3 mat = glm::mat3(glm::vec3(_scale.x, 0.f, 0.f), glm::vec3(0.f, _scale.y, 0.f), glm::vec3(0.f, 0.f, _scale.z));
	glm::vec3 right = getLocalRight();
	this->_modelMatrix = glm::mat3(right, _up, _forward) * mat;
	this->_modelMatrix[3] = glm::vec4(_position, 1.0f);
	if (this->_parent != nullptr) {
		this->_modelMatrix = this->_parent->_modelMatrix * this->_modelMatrix;
	}
}


void FANode::addChild(FANode *child) {
	//Use set child to update child
	child->setParent(this);
}

std::vector<FANode *> FANode::getAllChildren() {
	std::vector<FANode *> v;
	for (FANode *child : _children) {
		for (FANode *n : child->getAllChildren()) {
			v.push_back(n);
		}
	}
	v.push_back(this);
	return v;
}

void FANode::setParent(FANode *parent) {
	//Remove any old parent and set the new one
	this->removeFromParent();
	if (parent != nullptr) { //Verify there is a new parent
		this->_parent = parent;
		parent->_children.push_back(this);
	}
}
FANode* FANode::getParent() {
	return this->_parent;
}

void FANode::removeFromParent() {
    if (this->_parent != nullptr) {
        this->_parent->removeChild(this);
    }
}
/* Remove the node from the tree. Moving children to it's current parent
*/
void FANode::removeNode() {
	//Move children to the parent
	for (unsigned int i = 0; i < _children.size(); i++)
		_children[i]->setParent(_parent);
	removeFromParent();
}

void FANode::removeChild(FANode *node) {
    FANode *n = nullptr;
    for (unsigned int i = 0; i < _children.size(); i++) {
        n = _children[i];
        if (n == node) {
            _children.erase(_children.begin()+i);
            return;
        }
    }
}

void FANode::update(float dT) {
	if (!_isActive)
		return;
	onUpdate(dT);
	calcModelMatrix();
	updateChildren(dT);
}

void FANode::render(FrameData &fData) {
	if (!_isActive)
		return;
	onRender(fData);
	// calcModelMatrix();
	for (FANode *node : _children)
		node->render(fData);
}

void FANode::updateChildren(float dT)
{
	for (FANode *node : _children)
		node->update(dT);
}

void FANode::init()
{
	calcModelMatrix();
}
void FANode::reOrthogonalize()
{
	//Orthogonalize forward against up: (Gram-Schmidt):
	glm::vec3 forw = _forward - _up * glm::dot(_forward, _up);
	_forward = glm::normalize(forw);
}
void FANode::face(glm::vec3 point)
{
	setForward(point - glm::vec3(getWorldPos()));
}
void FANode::setForward(glm::vec3 axis)
{
	//Calculates the orientation vectors. Axis needs to be moved into local space
	glm::vec3 forw = glm::normalize(axis);
	glm::vec3 right = glm::cross(glm::vec3(0.f, 1.f, 0.f), forw);
	_up = glm::cross(forw, right);
	_forward = forw;
}
glm::vec3 FANode::getLocalRight() const
{
	return glm::cross(_up, _forward);
}

void FANode::setRotEuler(glm::vec3 xyz)
{
	glm::mat3 mat = glm::orientate3(glm::vec3(glm::radians(xyz.x), glm::radians(xyz.z), glm::radians(xyz.y)));
	_forward = mat[2];
	_up = mat[1];
}

void FANode::rotateX(float f) {
	glm::quat q = glm::angleAxis(f, getLocalRight());
	_up = q * _up;
	_forward = q * _forward;
	reOrthogonalize();
}

void FANode::rotateY(float f) {
	glm::quat q = glm::angleAxis(f, glm::vec3(0, 1.f, 0.f));
	_up = q * _up;
	_forward = q * _forward;
	reOrthogonalize();
}

void FANode::rotateZ(float f) {
	glm::quat q = glm::angleAxis(f, _forward);
	_up = q * _up;
	_forward = q * _forward;
	reOrthogonalize();
}

void FANode::rotate(glm::vec3 r) {

	glm::quat q = glm::angleAxis(r.y, _up);
	q *= glm::angleAxis(r.x, getLocalRight());
	q *= glm::angleAxis(r.z, _forward);
	_up = q * _up;
	_forward = q * _forward;
	reOrthogonalize();
}

#pragma region Set/Get vars

void FANode::setPositionX(float x) {
    _position.x = x;
}

void FANode::setPositionY(float y) {
	_position.y = y;
}

void FANode::setPositionZ(float z) {
	_position.z = z;
}

void FANode::setPosition(float x, float y, float z) {
	this->_position = glm::vec3(x, y, z);
}

void FANode::setPosition(glm::vec3 pos) {
	_position = pos;
}

void FANode::moveX(float x) {
	_position.x += x;
}

void FANode::moveY(float y) {
	_position.y += y;
}

void FANode::moveZ(float z) {
	_position.z += z;
}

void FANode::move(glm::vec3 p) {
	_position += p;
}

void FANode::setScale(float p) {
    _scale = glm::vec3(p,p,p);
}

void FANode::setScale(glm::vec3 p) {
	_scale = p;
}

void FANode::setScale(float x, float y) {
	_scale.x = x;
	_scale.y = y;
}

float FANode::getX()  {
    return _position.x;
}

float FANode::getY()  {
    return _position.y;
}

float FANode::getZ()  {
    return _position.z;
}


glm::vec3 FANode::getScale() const {
    return _scale;
}

glm::vec3 FANode::getPosition() const {
    return _position;
}
glm::vec3 FANode::getLocalForward() const
{
	return _forward;
}
glm::vec3 FANode::getLocalUp() const
{
	return _up;
}

const glm::mat4& FANode::getModelMatrix()
{
	return _modelMatrix;
}
glm::vec4 FANode::getWorldPos() const
{
	return _modelMatrix[3];
}

glm::vec3 FANode::getForward() const
{
	return glm::normalize(glm::vec3(_modelMatrix[2]));
}
glm::vec3 FANode::getUp() const {
	return glm::normalize(glm::vec3(_modelMatrix[1]));
}
glm::vec3 FANode::getRight() const {
	return glm::normalize(glm::vec3(_modelMatrix[0]));
}

float FANode::getDistance(FANode const & other) const
{
	return glm::length(getWorldPos() - other.getWorldPos());
}

float FANode::getDistance(glm::vec4 const &pos) const
{
	return glm::length(getWorldPos() - pos);
}

#pragma endregion
