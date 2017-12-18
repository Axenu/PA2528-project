#include "gl_include.h"
#include <iostream>
#define GLM_FORCE_RADIANS
#include <IL/il.h>
#include <glm/glm.hpp>
#include "Event/Input/InputManager.h"
#include "Event/EventManager.h"
// #include "Game/Setting.h"
// #include "Game/Game.h"
// #include "Game/TestGame.h"
#include "gui/Button.h"
#include "gui/Manager.h"
#include "gui/Views/ProjectView.h"
#include "gui/Views/MainMenuView.h"
#include "StaticVars.h"
#include "projectScene.h"
// #include "math/MathFunctions.h"
// #include "Config.h"
// #include "Sound/SoundManager.h"




#include "PA2528-2\TrackMemory.h"
#include "PA2528-2\PoolAllocator.h"
#include "PA2528-2\MemoryTracker.h"
#include "PA2528-3\PackageReader.hpp"
#include "PA2528-3\ResourceManager.hpp"
#include "PA2528-3\ThreadPool.hpp"

void setupWindow()
{
	//Memory leak debug
#if defined(DEBUG) && !defined(__APPLE__)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//Random seed
	// seed(1000);
	// Init glfw
	// gl::CheckGLErrors("GLError before glfwinit");
	if (!glfwInit())
	{
		std::cout << "GLFW init failed!" << std::endl;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_DECORATED, true);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL playground", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "GLFW creation of window failed!" << std::endl;
	}
	glfwMakeContextCurrent(window);
	gl::CheckGLErrors("GLError before glew");

#ifndef __APPLE__
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
#endif

	//init DevIL
	ilInit();

	//Set GL vars
	gl::CheckGLErrors("GLError before setting variables: Main");
	glEnable(GL_DEPTH_TEST);//Enable depth testinz
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);//Enable face culling
	glCullFace(GL_BACK);
	glEnable(GL_BLEND); //Enable alpha on gui elements. Could be done every frame on render?
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gl::CheckGLErrors("Init stage failed: Main");

	EventManager eventManager;
	InputManager iManager(window, &eventManager);

	//init dt calculation
	float lastTime = (float)glfwGetTime();
	float currentTime;
	float dT;
	float passedFrames = 0.0f;
	float passedTime = 0.0f;
	float FPS;

	gui::ProjectView* guiScene = new gui::ProjectView(&eventManager, &FPS);
	gui::Manager guiManager(&eventManager);
	guiManager.setWindowSize(1280, 720);
	guiManager.setView(guiScene);

	//setup our scene:
	ProjectScene ps(&eventManager);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.20703125f, 0.29296875f, 0.28125f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Calculate dt
		currentTime = (float)glfwGetTime();
		dT = currentTime - lastTime;
		lastTime = currentTime;
		passedTime += dT;
		passedFrames += 1;
		if (passedTime > 0.2f)
		{
			FPS = 1.0f / (passedTime / passedFrames);
			passedTime = 0.0f;
			passedFrames = 0.0f;
		}
		dT = std::fminf(dT, 0.1f); //Can't have to large dt!

		ps.update(dT);
		ps.render();

		guiManager.update(dT);
		guiManager.render(dT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		// unlock fps
		glfwSwapInterval(1);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
}

int main()
{
	// gl::CheckGLErrors("GLError at start");


	//load settings
	// Config::loadConfig("resources/settings.conf");
	//set mastetr volume
	// SoundManager::getInstance().setGlobalVolume(Config::masterVolume);
	// if (!Config::hasSound)
	// SoundManager::getInstance().setGlobalVolume(0.0f);
	//setup window and run game
	setupWindow();

#ifndef __APPLE__
	int i = 0;
	std::cin >> i;
#endif

	return 0;
}
