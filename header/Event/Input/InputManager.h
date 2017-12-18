#pragma once

#include "gl/glInclude.h"
#include "Event/EventManager.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include <functional>
#include <iostream>
#include <map>

class KeyboardEvent : public Event
{
public:
    KeyboardEvent(int keyID, int action) : _keyID(keyID), _action(action) {};

    int getKey() const {return _keyID;}
    int getAction() const {return _action;}

private:
    int _keyID;
    int _action;
};

class MouseMoveEvent : public Event
{
public:
    MouseMoveEvent(double x, double y, double diffX, double diffY) : _x(x), _y(y), _diffX(diffX), _diffY(diffY) {};

    int getX() const {return (int)_x;}
    int getY() const {return (int)_y;}
	int getDiffX() const { return (int)_diffX; }
	int getDiffY() const { return (int)_diffY; }
    glm::vec2 getPos() const {return glm::vec2(_x, _y);}

private:
    double _x;
    double _y;
	double _diffX, _diffY;
};

class MouseClickEvent : public Event
{
public:
    MouseClickEvent(int keyID, int action) : _keyID(keyID), _action(action) {};

    int getKey() const {return _keyID;}
    int getAction() const {return _action;}

private:
    int _keyID;
    int _action;
};

class cursorModeChangeEvent : public Event
{
public:
    cursorModeChangeEvent(int state) : _state(state) {};

    int getState() const {return _state;}

private:
    int _state;
};

class QuitGameEvent : public Event
{
public:
    QuitGameEvent() {};
};

class ResizeWindowEvent : public Event
{
public:
    int _width;
    int _height;
    ResizeWindowEvent(int width, int height);
    ~ResizeWindowEvent();
};

class InputManager
{
public:
    InputManager(GLFWwindow *window, EventManager* manager);
    ~InputManager();

    void switchCursorMode(const cursorModeChangeEvent& event);
    void quitGame(const QuitGameEvent& event);
    void resizeWindow(const ResizeWindowEvent& event);
    // int getCursorMode();

    EventManager* getManager();
	double _oldMouseX, _oldMouseY;
private:
    EventManager* _eventManager;
    GLFWwindow* _window;
    int _cursorMode;
};
