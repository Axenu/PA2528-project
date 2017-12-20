#ifndef __First__ProjectScene__
#define __First__ProjectScene__

#include "Game/Scene/Scene.h"
#include "Event/Input/InputManager.h"
#include "Game/FATexture.h"
#include "gui/ProgressBar.h"
#include "gui/Manager.h"

#include <list>
#include <vector>
#include <array>
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
	void loadChunk(float x, bool isLeft);
	void handlePendingChunkLoads();
	bool _moveCam = false;

private:
	void keyCallback(const KeyboardEvent& event);
	void mouseClickCallback(const MouseClickEvent &event);
	void mouseMoveCallback(const MouseMoveEvent &event);
	void updateChunks(bool isMovingLeft);

	EventManager *_eventManager;
	bool _isADown = false;
	bool _isDDown = false;
	float _xPos = 0.f;

	struct MeshLoad {
		MeshLoad(Promise<SharedPtr<Mesh>> m, Promise<SharedPtr<Texture>> t) : mesh(m), texture(t) {
		}
		Promise<SharedPtr<Mesh>> mesh;
		Promise<SharedPtr<Texture>> texture;
		FAModel* model = nullptr;
	};

	struct ChunkLoad {
		std::array<std::array<SharedPtr<MeshLoad>, 12>, 12> meshLoads;
		float xPos;
		float isLeft;
		bool isDone;
	};

	std::list<ChunkLoad> _chunks;
	std::vector<gui_t> _meshGuis;
	std::vector<gui_t> _textureGuis;
};

#endif
