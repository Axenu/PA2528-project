#ifndef __First__ProjectScene__
#define __First__ProjectScene__

#include "Game/Scene/Scene.h"
#include "Event/Input/InputManager.h"

class ProjectScene : public Scene {
public:
    ProjectScene(EventManager* manager);
    ~ProjectScene();

	void update(float dT) override;

private:
	void keyCallback(const KeyboardEvent& event);

	EventManager *_eventManager;
	bool _isADown = false;
	bool _isDDown = false;
};

#endif
