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
	std::cout << event.getKey() << std::endl;
}

ProjectScene::~ProjectScene(){

}
