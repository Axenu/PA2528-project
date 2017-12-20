#pragma once
#include<vector>
#include <iostream>
// #include"Game/Objects/GameObject.h"
// #include"Game/Objects/LootObject.h"
// #include"DrawFrame.h"
#include"node.h"
#include "Game/camera.h"
#include "Render/FrameObject.h"
// #include "Game/Scene/QuadTree.h"
// #include "Game/Objects/PointLightObject.h"


/* Scene holding the objects
*/
class Scene
{
private:
	/* Data ownership of objects in the scene.
	*/
	// std::vector<GameObject*> _staticObjects;
	// std::vector<GameObject*> _dynamicObjects;
	/* Scene node objects.
	*/
	// std::vector<FANode*> _children;
	/* Reference to the current sccene camera */

    FANode *_root;
	FrameObject *_fObject;
	/* Culling tree for static objects */
	// QuadTreeNode _quadTree;

protected:
    Camera* _cam;
	// GameObject* _rootObject;
	/* The scene tree root.
	*/
	// std::unique_ptr<Node> _root;
public:
	// Scene(std::unique_ptr<GameObject> &root, AABB sceneBounds);
    Scene();
	virtual ~Scene();

	void setCamera(Camera *cam);
	Camera& getCamera();
	virtual void update(float dT);
    void render();
	void addNode(FANode *object);
	void removeNode(FANode *object);
	virtual void setUpFrameObject();

};