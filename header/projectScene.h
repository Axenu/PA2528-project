#ifndef __First__ProjectScene__
#define __First__ProjectScene__

#include "Game/Scene/Scene.h"
#include "Event/Input/InputManager.h"
#include "Game/FATexture.h"

#include <list>
#include <vector>
class FAModel;


#include "PA2528-3/Promise.hpp"
#include "PA2528-3/Texture.hpp"
#include "PA2528-3/Mesh.hpp"
#include "PA2528-3/gui_t.hpp"

class ProjectScene : public Scene {
public:
    ProjectScene(EventManager* manager);
    ~ProjectScene();

	void update(float dT) override;


private:
	void loadMesh(float x, bool isLeft);
	void handlePendingMeshLoads();

private:
	void keyCallback(const KeyboardEvent& event);
	void updateMeshesLeft();
	void updateMeshesRight();

	EventManager *_eventManager;
	bool _isADown = false;
	bool _isDDown = false;
	float _xPos = 0.f;

	struct MeshLoad {
		MeshLoad(Promise<SharedPtr<Mesh>> m, Promise<SharedPtr<Texture>> t) : mesh(m), texture(t) {
		}
		Promise<SharedPtr<Mesh>> mesh;
		Promise<SharedPtr<Texture>> texture;
		float xPos;
		bool isLeft;
	};

	std::list<MeshLoad> _pendingMeshLoads;
	std::list<FAModel*> _models;
	std::vector<gui_t> _meshGuis;
	std::vector<gui_t> _textureGuis;
};

#endif
