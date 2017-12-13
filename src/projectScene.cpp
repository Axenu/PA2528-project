#include "projectScene.h"
#include "Game/FAModel.h"


ProjectScene::ProjectScene() : Scene() {
    _cam->moveZ(-2);
    FAMesh *mesh = new FAMesh("Chalice.obj");
    FAMaterial *material = new FAMaterial();
    FAModel *model = new FAModel(mesh, material);
    model->setCam(_cam);
    // model->setScale(0.1f);
    // model->moveZ(-3.f);
    this->addNode(model);
}
ProjectScene::~ProjectScene(){

}
