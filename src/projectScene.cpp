#include "projectScene.h"
#include "Game/FAModel.h"

#include "PA2528-3/ThreadPool.hpp"
#include "PA2528-3/PackageReader.hpp"
#include "PA2528-3/ResourceManager.hpp"
#include "PA2528-2/MemoryTracker.h"
#include "PA2528-2/PoolAllocator.h"


ProjectScene::ProjectScene(EventManager* manager) : Scene() {
	srand(NULL);
	PackageReader::setPackage("PA2528-3/package tool/res2");
	ThreadPool::initialize();
	ResourceManager::initialize();


	Array<PackageReader::MetaData> metaDatas = PackageReader::getMetaData();
	for (size_t i = 0; i < metaDatas.size; i++) {
		const PackageReader::MetaData& d = metaDatas.data[i];
		using T = PackageReader::MetaData::Type;
		switch(d.type) {
		case T::MESH: _meshGuis.push_back(d.gui); std::cout << d.gui << std::endl; break;
			case T::TEXTURE: _textureGuis.push_back(d.gui); break;
			default: break;
		}
	}



	//Promise<SharedPtr<Mesh>> mp5 = ResourceManager::aloadMesh(6722305721597800034);
	//Mesh* m = mp5.get().get();


	_cam->moveZ(-2);

	SharedPtr<Mesh> m = PackageReader::loadMesh(_meshGuis[0]);
	FAMesh *mesh = new FAMesh(m);
	FAMaterial *material = new FAMaterial();
	//material->setTexture(FATexture::getDefaultTexture());
	material->setColorMemUsage(100, 50);
	FAModel *model = new FAModel(mesh, material);
	model->setScale(1.0f);
	model->moveZ(-2.f);
	this->addNode(model);
	_models.push_back(model);

	//mesh = new FAMesh();
	//material = new FAMaterial(texture);
	//model = new FAModel(mesh, material);
	//model->moveX(0.5f);
	//this->addNode(model);
	//_models.push_back(model);

	//events
	_eventManager = manager;
	_eventManager->listen(this, &ProjectScene::keyCallback);
}

void ProjectScene::keyCallback(const KeyboardEvent& event)
{
	if (event.getAction() == GLFW_PRESS) {
		switch (event.getKey()) {
			case GLFW_KEY_A: _isADown = true; break;
			case GLFW_KEY_D: _isDDown = true; break;
			default:
				break;
		}
	}
	else if(event.getAction() == GLFW_RELEASE) {
		switch (event.getKey()) {
		case GLFW_KEY_A: _isADown = false; break;
		case GLFW_KEY_D: _isDDown = false; break;

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
			FAMaterial *material = new FAMaterial();
			material->setColorMemUsage(100, rand() % 100 + 1);  // placeholder test
			material->setTexture(-1);
			FAModel *model = new FAModel(mesh, material);

			model->setPositionX(it->xPos);
			// TODO: Remove this when meshes are fixed.
			model->setScale(0.001f);

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

	// TODO: Remove this when meshes are fixed.
	//meshGui = _meshGuis[0];
	//textureGui = _textureGuis[0];
	//

	MeshLoad load(ResourceManager::aloadMesh(meshGui), ResourceManager::aloadTexture(textureGui));
	load.xPos = x;
	load.isLeft = isLeft;
	_pendingMeshLoads.push_back(load);
}

void ProjectScene::updateMeshes(bool isMovingLeft) {
	if (isMovingLeft) {
		if (_models.front()->getX() - _xPos > 0.5f) {
			loadMesh(_xPos, true);

			//static SharedPtr<Mesh> m = ResourceManager::loadMesh(_meshGuis[2]);
			//FAMesh *mesh = new FAMesh(m);
			///*float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			//FAMaterialColor *material = new FAMaterialColor(color);*/
			//FAMaterialColor *material = new FAMaterialColor();
			//material->setColorMemFrag(100, 50); // 50% memory fragmentation
			//FAModel *model = new FAModel(mesh, material);
			//model->setPositionX(_xPos);
			//this->removeNode(_models.back());
			//_models.pop_back();
			//this->addNode(model);
			//_models.push_front(model);
		}
	}
	else {
		if (_models.back()->getX() - _xPos  > 0.5f) {
			// loadMesh(_xPos, false);

			FAMesh *mesh = new FAMesh("Chalice.obj");
			FAMaterial *material = new FAMaterial();
			FAModel *model = new FAModel(mesh, material);
			model->setPositionX(_xPos);
			this->removeNode(_models.front());
			_models.pop_front();
			this->addNode(model);
			_models.push_back(model);
			// change the model's color based on the internal fragmentation of it's memory
			material->setColorMemUsage(100, rand() % 100 + 1);  // placeholder test

		}
	}
}

ProjectScene::~ProjectScene() {

}
