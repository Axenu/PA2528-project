#include <Game/FAModel.h>

FAModel::FAModel() {

}

FAModel::FAModel(SharedPtr<FAMesh> mesh) {
	this->mesh = mesh;
}

FAModel::FAModel(SharedPtr<FAMesh> mesh, SharedPtr<FAMaterial> material) {
	this->mesh = mesh;
	this->material = material;
	// this->material->setViewProjectionwMatrix()
	// this->material.addVertexComponents(this->mesh.getAvaliableComponents());
	// this->material.create();
}

void FAModel::setMaterial(SharedPtr<FAMaterial> material) {
	this->material = material;
	// this->material.addVertexComponents(this->mesh.getAvaliableComponents());
	// this->material.create();
}

void FAModel::setMesh(SharedPtr<FAMesh> mesh) {
	this->mesh = mesh;
	// this->material.addVertexComponents(this->mesh.getAvaliableComponents());
	// this->material.create();
}

void FAModel::setMesh(std::string path) {
	// this->mesh = new FAMesh(path);
	// this->material.addVertexComponents(this->mesh.getAvaliableComponents());
	// this->material.create();
}

FAMaterial &FAModel::getMaterial() {
	return *this->material;
}

SharedPtr<FAMesh> FAModel::getMesh() const {
	return this->mesh;
}

glm::mat4 &FAModel::getModelMatrix() {
	return this->_modelMatrix;
}

void FAModel::onUpdate(float dt) {
	this->material->setModelMatrix(this->_modelMatrix);
}

void FAModel::onRender(FrameData &fData) {
	// this->material->setModelMatrix(this->_modelMatrix);
	//this->material->setViewProjectionwMatrix(fData.VPMatrix);
	material->bind(fData);
	mesh->render();
	// std::cout << "render" << std::endl;
}

FAModel::~FAModel() {
}
