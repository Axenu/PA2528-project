#include "projectScene.h"
#include "Game/FAModel.h"


ProjectScene::ProjectScene(EventManager* manager) : Scene() {
    _cam->moveZ(-2);

    FAMesh *mesh = new FAMesh();
    FAMaterial *material = new FAMaterial();
    material->setTexture(FATexture::getDefaultTexture());
    FAModel *model = new FAModel(mesh, material);
    // model->setScale(0.1f);
    // model->moveZ(-3.f);
    this->addNode(model);
	_models.push_back(model);

	mesh = new FAMesh("Chalice.obj");
	material = new FAMaterial();
	model = new FAModel(mesh, material);
	model->moveX(0.5f);
	this->addNode(model);
	_models.push_back(model);

	//events
	_eventManager = manager;
	_eventManager->listen(this, &ProjectScene::keyCallback);
}

void ProjectScene::keyCallback(const KeyboardEvent& event)
{
	static constexpr int A_KEY = 65;
	static constexpr int D_KEY = 68;
	static constexpr int DOWN_ACTION = 1;
	static constexpr int UP_ACTION = 0;
	if (event.getAction() == DOWN_ACTION) {
		switch (event.getKey()) {
			case A_KEY: _isADown = true; break;
			case D_KEY: _isDDown = true; break;

			default:
				break;
		}
	}
	else if(event.getAction() == UP_ACTION) {
		switch (event.getKey()) {
		case A_KEY: _isADown = false; break;
		case D_KEY: _isDDown = false; break;

		default:
			break;
		}
	}
}

void ProjectScene::update(float dT) {
	static constexpr float SPEED = 1.f;
	float distance = SPEED * dT;
	if (_isADown) {
		_xPos -= distance;
		_cam->moveX(distance);
		updateMeshesLeft();
	}
	else if (_isDDown) {
		_xPos += distance;
		_cam->moveX(-distance);
		updateMeshesRight();
	}
	Scene::update(dT);
}

void ProjectScene::updateMeshesLeft() {
	if (_models.front()->getX() - _xPos > 0.5f) {
		FAMesh *mesh = new FAMesh("Chalice.obj");
		FAMaterial *material = new FAMaterial();
		FAModel *model = new FAModel(mesh, material);
		model->setPositionX(_xPos);
		this->removeNode(_models.back());
		_models.pop_back();
		this->addNode(model);
		_models.push_front(model);
	}

}

void ProjectScene::updateMeshesRight() {
	if (_xPos - _models.back()->getX() > 0.5f) {
		FAMesh *mesh = new FAMesh("Chalice.obj");
		FAMaterial *material = new FAMaterial();
		FAModel *model = new FAModel(mesh, material);
		model->setPositionX(_xPos);
		this->removeNode(_models.front());
		_models.pop_front();
		this->addNode(model);
		_models.push_back(model);
	}

}

ProjectScene::~ProjectScene(){

}
