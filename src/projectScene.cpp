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
	ResourceManager::setMemoryLimit(100000000);


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

	GLuint texture = FATexture::getTexture(PackageReader::loadTexture(_textureGuis[0]));
	SharedPtr<Mesh> m = PackageReader::loadMesh(_meshGuis[2]);
	FAMesh *mesh = new FAMesh(m);
	FAMaterial *material = new FAMaterial(texture);
	//material->setTexture(FATexture::getDefaultTexture());
	material->setColorMemUsage(m->memAllocated, m->memUsed);
	FAModel *model = new FAModel(mesh, material);
	//model->setScale(0.001f);
	//model->rotateX(0.5f);
	model->moveZ(-2.f);
	this->addNode(model);
	_models.push_back(model);

	mesh = new FAMesh();
	//GLuint texture = FATexture::getTexture(PackageReader::loadTexture(_textureGuis[0]));
	material = new FAMaterial(texture);
	model = new FAModel(mesh, material);
	model->moveX(0.5f);
	this->addNode(model);
	_models.push_back(model);

	//events
	_eventManager = manager;
	_eventManager->listen(this, &ProjectScene::keyCallback);
	_eventManager->listen(this, &ProjectScene::mouseClickCallback);
	_eventManager->listen(this, &ProjectScene::mouseMoveCallback);
}

void ProjectScene::keyCallback(const KeyboardEvent& event)
{
	if (event.getAction() == GLFW_PRESS) {
		switch (event.getKey()) {
			case GLFW_KEY_A: _isADown = true; break;
			case GLFW_KEY_D: _isDDown = true; break;
			case GLFW_KEY_W: _cam->moveZ(0.5); break;
			case GLFW_KEY_S: _cam->moveZ(-0.5);; break;
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

	ResourceManager::garbageCollect();
}

void ProjectScene::handlePendingMeshLoads() {
	for (auto it = _pendingMeshLoads.begin(); it != _pendingMeshLoads.end();) {
		if (it->mesh.isReady() && it->texture.isReady()) {
			SharedPtr<FAMesh> mesh;
			for (const FAModel* model : _models) {
				if (model->getMesh()->get_aiMesh() == it->mesh.get()) {
					mesh = model->getMesh();
				}
			}
			if (mesh == nullptr) {
				mesh = new FAMesh(it->mesh.get());
			}
			FATexture *texture = new FATexture(it->texture.get());
			FAMaterial *material = new FAMaterial();
			material->setColorMemUsage(it->mesh.get()->memAllocated, it->mesh.get()->memUsed);  // placeholder test
			material->setTexture(-1);
			FAModel *model = new FAModel(mesh, material);

			model->setPositionX(it->xPos);

			if (it->isLeft) {
				if (_models.size() > 20) {
					this->removeNode(_models.back());
					delete _models.back();
					_models.pop_back();
				}
				this->addNode(model);
				_models.push_front(model);
			}
			else {
				if (_models.size() > 20) {
					this->removeNode(_models.front());
					delete _models.front();
					_models.pop_front();
				}
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
			loadMesh(_xPos, true);

		}
	}
	else {
		if (_xPos - _models.back()->getX() > 0.5f) {
			loadMesh(_xPos, false);
		}
	}
}

void ProjectScene::mouseClickCallback(const MouseClickEvent &event) {
	if (event.getKey() == GLFW_MOUSE_BUTTON_1) {
		if (event.getAction() == GLFW_PRESS) {
			_moveCam = true;
		} else {
			_moveCam = false;
		}
	}
}

void ProjectScene::mouseMoveCallback(const MouseMoveEvent &event) {
	if (_moveCam) {
		_cam->rotateX(event.getDiffY() * 0.001f);
		_cam->rotateZ(event.getDiffX() * 0.001f);
	}
}

ProjectScene::~ProjectScene() {

}
