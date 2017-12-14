#include "projectScene.h"
#include "Game/FAModel.h"


ProjectScene::ProjectScene(EventManager* manager) : Scene() {
    _cam->moveZ(-2);
    FAMesh *mesh = new FAMesh("Chalice.obj");
    FAMaterial *material = new FAMaterial();
    FAModel *model = new FAModel(mesh, material);
    model->setCam(_cam);
    // model->setScale(0.1f);
    // model->moveZ(-3.f);
    this->addNode(model);

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
	if (_isADown) {
		_cam->moveX(SPEED * dT);
	}
	else if (_isDDown) {
		_cam->moveX(-SPEED * dT);
	}
	Scene::update(dT);
}

ProjectScene::~ProjectScene(){

}
