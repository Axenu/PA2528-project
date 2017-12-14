#pragma once
#include<vector>
// #include"Game/Objects/GameObject.h"
// #include"Game/Objects/LootObject.h"
// #include"DrawFrame.h"
#include"node.h"
#include "Game/camera.h"
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
	~Scene();

	void setCamera(Camera *cam);
	Camera& getCamera();
	void update(float dT);
    void render();
	void addNode(FANode *object);
	void removeNode(FANode *object);
	/* Populates the DrawFrame by fetching and culling the scene drawables
	*/
	// void fetchDrawables(DrawFrame &dF);

	// void fetchDrawables(DrawFrame &dF, AABB &aabb);

	/* Get a list of objects inside the bounds and of the specific enum type. */
	// template<type::GameObjectType Type>
	// std::vector<GameObject*> fetchStaticObjects(const AABB& aabb);
	/* Get a list of objects inside the bounds and of the specific class type. */
	// template<typename T>
	// std::vector<T*> fetchDynamicObjects(const AABB& aabb);

	/* Fetch a set of game objects compared with the function */
	// std::vector<GameObject*> fetchStaticObjects(const AABB& aabb, bool *(compFunc)(GameObject* obj));
	// int loot(float pickDist);
	// GameObject* pick(float pickDist);
	// GameObject* pick(float pickDist, Node *ignoreNode);
	// bool takeOverGuard(float maxDist);
	// void getDynObjects(std::vector<GameObject*> &list, const glm::mat4 & mat);
	//get dynamic objects for a ligth source
	// void getDynObjects(std::vector<GameObject*> &list, AABB &iaabb);

};


// #pragma region Template fetch funcs
//
// template<type::GameObjectType Type>
// bool isGameType(GameObject *obj)
// {
// 	return obj->_type == Type;
// }
//
// template<type::GameObjectType Type>
// std::vector<GameObject*> Scene::fetchStaticObjects(const AABB& aabb)
// {
// 	return fetchObjects(aabb, &isGameType<Type>);
// }
// template<typename T>
// std::vector<T*> Scene::fetchDynamicObjects(const AABB& aabb)
// {
// 	static_assert(std::is_base_of<GameObject, T>::value, "T param must be derived from GameObject");
// 	std::vector<T*> list;
// 	for (unsigned int i = 0; i < _dynamicObjects.size(); i++)
// 	{
// 		T* ptr = dynamic_cast<T*>(_dynamicObjects[i]);
// 		//If cast successfull and is inside aabb add obj to list
// 		if (ptr && AABBvAABB(_dynamicObjects[i]->getAABB(), aabb))
// 			list.push_back(ptr);
// 	}
// 	return list;
// }
// #pragma endregion
