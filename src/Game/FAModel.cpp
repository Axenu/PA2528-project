#include <Game/FAModel.h>

FAModel::FAModel() {

}

FAModel::FAModel(FAMesh *mesh) {
	this->mesh = mesh;
}

FAModel::FAModel(FAMesh *mesh, FAMaterial *material) {
	this->mesh = mesh;
	this->material = material;
	// this->material.addVertexComponents(this->mesh.getAvaliableComponents());
	// this->material.create();
}

void FAModel::setMaterial(FAMaterial *material) {
	this->material = material;
	// this->material.addVertexComponents(this->mesh.getAvaliableComponents());
	// this->material.create();
}

void FAModel::setMesh(FAMesh *mesh) {
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

const FAMesh &FAModel::getMesh() const {
	return *this->mesh;
}

glm::mat4 &FAModel::getModelMatrix() {
	return this->_modelMatrix;
}

void FAModel::onUpdate(float dt) {
	// this->material->setModelMatrix(this->modelMatrix);
	// this->mesh->update(dt);
}

FAModel::~FAModel() {
}
