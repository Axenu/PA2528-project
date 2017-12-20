#include "projectScene.h"
#include "Game/FAModel.h"

#include "PA2528-3/ThreadPool.hpp"
#include "PA2528-3/PackageReader.hpp"
#include "PA2528-3/ResourceManager.hpp"
#include "PA2528-2/MemoryTracker.h"
#include "PA2528-2/PoolAllocator.h"


ProjectScene::ProjectScene(EventManager* manager) : Scene() {
	srand(NULL);
	PackageReader::setPackage("Resources/res2");
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
	//_chunks.push_back(model);

	mesh = new FAMesh();
	//GLuint texture = FATexture::getTexture(PackageReader::loadTexture(_textureGuis[0]));
	material = new FAMaterial(texture);
	model = new FAModel(mesh, material);
	model->moveX(0.5f);
	this->addNode(model);
	//_chunks.push_back(model);

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
		updateChunks(true);
	}
	else if (_isDDown) {
		_xPos += distance;
		_cam->moveX(-distance);
		updateChunks(false);
	}
	handlePendingChunkLoads();
	Scene::update(dT);

	ResourceManager::garbageCollect();
}

void ProjectScene::handlePendingChunkLoads() {
	for (auto it = _chunks.begin(); it != _chunks.end(); it++) {
		if (!it->isDone) {
			bool areAllLoadsReady = true;
			for (size_t x = 0; x < it->meshLoads.size(); x++) {
				for (size_t y = 0; y < it->meshLoads[x].size(); y++) {
					SharedPtr<MeshLoad>& load = it->meshLoads[x][y];
					if (load->model == nullptr) {
						if (!load->mesh.isReady() || !load->texture.isReady()) {
							areAllLoadsReady = false;
						}
						else {
							SharedPtr<FAMesh> mesh;
							for (const ChunkLoad& chunk : _chunks) {
						
								FAModel* model = nullptr;
								for (const auto& column : chunk.meshLoads) {
									for (const SharedPtr<MeshLoad>& m : column) {
										if (m->model) {
											model = m->model;
											break;
										}
									}
									if (model) {
										break;
									}
								}
								if(model && model->getMesh()->get_aiMesh() == load->mesh.get()) {
									mesh = model->getMesh();
									break;
								}
							}
							if (mesh == nullptr) {
								mesh = new FAMesh(load->mesh.get());
							}
							FATexture *texture = new FATexture(load->texture.get());
							FAMaterial *material = new FAMaterial();
							material->setColorMemUsage(load->mesh.get()->memAllocated, load->mesh.get()->memUsed);  // placeholder test
							material->setTexture(-1);
							FAModel *model = new FAModel(mesh, material);

							static constexpr float X_INCREMENT = ChunkLoad::WIDTH / float(ChunkLoad::SIZE);
							static constexpr float Y_INCREMENT = ChunkLoad::HEIGHT / float(ChunkLoad::SIZE);

							float xPos = it->xPos - ChunkLoad::WIDTH / 2.f + float(x) * X_INCREMENT;
							float yPos = -ChunkLoad::HEIGHT / 2.f + float(y) * Y_INCREMENT;
							model->setPositionX(xPos);
							model->setPositionY(yPos);

							load->model = model;

							this->addNode(model);
						}

					}
				}
			}
			it->isDone = areAllLoadsReady;
		}
	}
}

void ProjectScene::loadChunk(bool isLeft) {
	gui_t meshGui = _meshGuis[rand() % _meshGuis.size()];
	gui_t textureGui = _textureGuis[rand() % _textureGuis.size()];
	
	ChunkLoad chunk;

	for (auto& column : chunk.meshLoads) {
		for (SharedPtr<MeshLoad>& load : column) {
			load = new MeshLoad(ResourceManager::aloadMesh(meshGui), ResourceManager::aloadTexture(textureGui));
		}
	}
	
	chunk.isLeft = isLeft;
	if (isLeft) {
		chunk.xPos = _leftChunkX - ChunkLoad::WIDTH;
		_leftChunkX = chunk.xPos;
		_chunks.push_front(chunk);
	}
	else {
		chunk.xPos = _rightChunkX + ChunkLoad::WIDTH;
		_rightChunkX = chunk.xPos;
		_chunks.push_back(chunk);
	}
}

void ProjectScene::removeChunk(ChunkLoad& chunk) {
	for (auto& column : chunk.meshLoads) {
		for (SharedPtr<MeshLoad>& load : column) {
			if (load->model) {
				this->removeNode(load->model);
				delete load->model;
			}
		}
	}
}

void ProjectScene::updateChunks(bool isMovingLeft) {
	if (isMovingLeft) {
		while(_leftChunkX - _xPos > ChunkLoad::WIDTH) {
			loadChunk(true);
		}

		while (!_chunks.empty() && std::fabs(_chunks.back().xPos - _xPos) > ChunkLoad::WIDTH * 2.f) {
			removeChunk(_chunks.back());
			_chunks.pop_back();
			_rightChunkX -= ChunkLoad::WIDTH;
		}
	}
	else {
		while (_xPos - _rightChunkX > ChunkLoad::WIDTH) {
			loadChunk(false);
		}

		while (!_chunks.empty() && std::fabs(_chunks.front().xPos - _xPos) > ChunkLoad::WIDTH * 2.f) {
			removeChunk(_chunks.front());
			_chunks.pop_front();
			_leftChunkX += ChunkLoad::WIDTH;
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
