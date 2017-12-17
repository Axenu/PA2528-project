#include "projectScene.h"
#include "Game/FAModel.h"

#include "PA2528-3/ThreadPool.hpp"
#include "PA2528-3/PackageReader.hpp"
#include "PA2528-3/ResourceManager.hpp"


ProjectScene::ProjectScene(EventManager* manager) : Scene() {
	srand(NULL);
	PackageReader::setPackage("../PA2528-3/package tool/res");
	ThreadPool::initialize();
	ResourceManager::initialize();

	Array<PackageReader::MetaData> metaDatas = PackageReader::getMetaData();
	for (size_t i = 0; i < metaDatas.size; i++) {
		const PackageReader::MetaData& d = metaDatas.data[i];
		using T = PackageReader::MetaData::Type;
		switch(d.type) {
			case T::MESH: _meshGuis.push_back(d.gui); break;
			case T::TEXTURE: _textureGuis.push_back(d.gui); break;
			default: break;
		}
	}

	//Promise<SharedPtr<Mesh>> mp5 = ResourceManager::aloadMesh(6722305721597800034);
	//Mesh* m = mp5.get().get();
	 


    _cam->moveZ(-2);

	FAMesh *mesh = new FAMesh();
	FAMaterial *material = new FAMaterial();
	material->setTexture(FATexture::getDefaultTexture());
	FAModel *model = new FAModel(mesh, material);
	// model->setScale(0.1f);
	// model->moveZ(-3.f);
	this->addNode(model);
	_models.push_back(model);

	mesh = new FAMesh();
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
		updateMeshes(true);
	}
	else if (_isDDown) {
		_xPos += distance;
		_cam->moveX(-distance);
		updateMeshes(false);
	}
	handlePendingMeshLoads();
	Scene::update(dT);
}

void ProjectScene::handlePendingMeshLoads() {
	for (auto it = _pendingMeshLoads.begin(); it != _pendingMeshLoads.end();) {
		if (it->mesh.isReady() && it->texture.isReady()) {
			FAMesh *mesh = new FAMesh(it->mesh.get());
			FATexture *texture = new FATexture(it->texture.get());
			float color[4] = { 0.0f, 255.0f, 0.0f, 255.0f };
			FAMaterialColor *material = new FAMaterialColor(color);
			material->setTexture(-1);
			FAModel *model = new FAModel(mesh, material);
			
			model->setPositionX(it->xPos);

			if (it->isLeft) {
				this->removeNode(_models.back());
				_models.pop_back();
				this->addNode(model);
				_models.push_front(model);
			}
			else {
				this->removeNode(_models.front());
				_models.pop_front();
				this->addNode(model);
				_models.push_back(model);
			}

			_pendingMeshLoads.erase(it++);
		}
		else {

			it++;
		}

	}
}

void ProjectScene::loadMesh(float x, bool isLeft) {
	gui_t meshGui = _meshGuis[rand() % _meshGuis.size()];
	gui_t textureGui = _textureGuis[rand() % _textureGuis.size()];
	
	MeshLoad load(ResourceManager::aloadMesh(meshGui), ResourceManager::aloadTexture(textureGui));
	load.xPos = x;
	load.isLeft = isLeft;
	_pendingMeshLoads.push_back(load);
}

void ProjectScene::updateMeshes(bool isMovingLeft) {
	if (isMovingLeft) {
		if (_models.front()->getX() - _xPos > 0.5f) {
			// loadMesh(_xPos, true);

			FAMesh *mesh = new FAMesh("Chalice.obj");
			/*float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			FAMaterialColor *material = new FAMaterialColor(color);*/
			FAMaterialColor *material = new FAMaterialColor();
			material->setColorMemFrag(100, 50); // 50% memory fragmentation
			FAModel *model = new FAModel(mesh, material);
			model->setPositionX(_xPos);
			this->removeNode(_models.back());
			_models.pop_back();
			this->addNode(model);
			_models.push_front(model);
		}
	}
	else {
		if (_xPos - _models.back()->getX() > 0.5f) {
			// loadMesh(_xPos, false);

			FAMesh *mesh = new FAMesh("Chalice.obj");
			/*float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			FAMaterialColor *material = new FAMaterialColor(color);*/
			FAMaterialColor *material = new FAMaterialColor();
			material->setColorMemFrag(100, 0);  // 0% memory fragmentation
			FAModel *model = new FAModel(mesh, material);
			model->setPositionX(_xPos);
			this->removeNode(_models.front());
			_models.pop_front();
			this->addNode(model);
			_models.push_back(model);
		}
	}
}

ProjectScene::~ProjectScene(){

}
