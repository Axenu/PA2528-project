#include "Game/Scene/Scene.h"
// #include <memory>


Scene::Scene() {
    _root = new FANode();
    _cam = new Camera(70, 1.5, 0.1, 10);
}
Scene::~Scene() {

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
	this->fObject = this->setUpFrameObject();
}

FrameObject *Scene::setUpFrameObject() {
	FrameData fData;
	fData.VPMatrix = _cam->VPMatrix;
	FrameObject *fObject = new FrameObject(fData);
	fObject->addObject(_root);
	return fObject;
}

void Scene::render() {
    //_root->render(this->fObject->fData);
	for (int i = 0; i < this->fObject->objects.size(); i++) {
		this->fObject->objects[i]->render(this->fObject->fData);
	}
}

void Scene::addNode(FANode *object) {
    _root->addChild(object);
}

void Scene::removeNode(FANode *object) {
    _root->removeChild(object);
}
