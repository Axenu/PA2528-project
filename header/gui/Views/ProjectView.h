#pragma once

#include <iostream>
#include <vector>
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "gui/Font.h"
#include "gui/View.h"
#include "gui/Button.h"
#include "Event/Input/InputManager.h"
#include "Event/Events.h"
// #include "gui/Views/LoadingView.h"
// #include "gui/Views/CreditsView.h"
// #include "staticVars.h"
#include "gui/ProgressBar.h"
#include "gui/Label.h"
// #include <Sound/Irrklang.h>
// #include "Sound/SoundManager.h"

namespace gui
{

	class ProjectView : public View
	{
	private:
		//passed from main
		EventManager * _eventManager;
		//owned and deleted by subclass View -> Element
		gui::Button* _quitButton;
		//pointer to float not owned by this
		float* _fps;
		ProgressBar *bar;

		// Progress bars displaying the current memory usage of allocators
		std::vector<gui::ProgressBar*> memoryUsageBars;
		std::vector<gui::Label*> memoryUsageLabels;

	public:
		ProjectView(EventManager* manager, float* fps);
		virtual ~ProjectView();

		void onRender(float dt);
		void onUpdate(float dt);
		void pauseView() {}
		void resumeView() {}

		virtual void initiate();

		void QuitGame(int action);
	};

}
