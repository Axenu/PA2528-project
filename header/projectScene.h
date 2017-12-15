#ifndef __First__ProjectScene__
#define __First__ProjectScene__

#include "Game/Scene/Scene.h"
#include "Event/Input/InputManager.h"
#include "Game/FATexture.h"

#include <list>
class FAModel;

class ProjectScene : public Scene {
public:
    ProjectScene(EventManager* manager);
    ~ProjectScene();

	void update(float dT) override;

private:
	void keyCallback(const KeyboardEvent& event);
	void updateMeshesLeft();
	void updateMeshesRight();

	EventManager *_eventManager;
	bool _isADown = false;
	bool _isDDown = false;
	float _xPos = 0.f;
	std::list<FAModel*> _models;
};

#endif
