#include "Game/Scene/Scene.h"
// #include <memory>


Scene::Scene() {
    _root = new FANode();
    _cam = new Camera(70, 1.5, 0.1, 10);
	_fObject = new FrameObject();
}
Scene::~Scene() {
	delete _fObject;
	delete _root;
	//delete _fObject;
}

void Scene::setCamera(Camera *cam) {
    this->_cam = cam;
}
Camera& Scene::getCamera() {
    return *_cam;
}
void Scene::update(float dT) {
    _cam->update(dT);
    _root->update(dT);
	//setup frameobject with framedata and render objects
    _fObject->clear();
    //delete this->fObject;
	this->setUpFrameObject();
}

void Scene::setUpFrameObject() {
	FrameData fData;
	fData.VPMatrix = _cam->VPMatrix;
	_fObject->setFrameData(fData);
	//_fObject->addObject(_root);
	for (int i = 0; i < _root->getNumberOfChildren(); i++) {
		_fObject->addObject(_root->getChild(i));
	}
	//std::cout << "update count: " << _root->getNumberOfChildren() << std::endl;
}

void Scene::render() {
	FANode *n;
	int count = 0;
	do
	{
		n = _fObject->getNext();
		if (n == nullptr) break;
		n->render(_fObject->_fData);
		count += 1;
	} while (true);
	//std::cout << "render count: " << count << std::endl;

    //_root->render(this->fObject->fData);
	//for (int i = 0; i < this->fObject->objects.size(); i++) {
	//	this->fObject->objects[i]->render(this->fObject->fData);
	//}
}

void Scene::addNode(FANode *object) {
    _root->addChild(object);
}

void Scene::removeNode(FANode *object) {
    _root->removeChild(object);
}
