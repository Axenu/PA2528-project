#ifndef __First__FAMesh__
#define __First__FAMesh__

#include "gl_include.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <assimp/mesh.h>
#include "PA2528-3/SharedPtr.hpp"
#include "PA2528-3/Mesh.hpp"
// #include "FAArmature.h"
// #include "FAMAterialComponent.h"
// #include "FAAABB.h"

class FAMesh {

private:

	// bool _hasNormal;
	// bool _hasColor;
	bool _hasUV = true;
	// bool _hasWeights;
	// bool _hasArmature;

	std::string objMaterailLib;
	std::string objMaterial;

	// std::vector<FAMaterialComponent *> avaliableVertexComponents;

	// FABone *rootBone;

	// std::vector<glm::mat4> invBindPose;
	// std::vector<FAAnimation *> animations;
	// FAArmature *armature;
	// FAAABB boundingBox;

	void loadOBJModel(std::string path);
	// void loadFAModel(std::string path);
	// void loadNewFAModel(std::string path);
	// void adjustPositions(FABone *b, glm::vec3 diff);
	// void setupBoneMatrices(FABone *b);
	// int calculateBoneMatrices(FABone *b, int i);

	//list avaliable material thins

public:
	FAMesh();
	FAMesh(std::string path);
	FAMesh(SharedPtr<Mesh> mesh);
	//FAMesh(aiMesh &aiMesh);
	// FAMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, bool hasNormal, bool hasColor);
	// FAMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, bool hasNormal, bool hasColor);

	virtual void render() const;
	virtual void update(float dt);

	// std::string getOBJMaterialLib();
	// std::string getOBJMaterial();
	// FAAABB getBounds();

	// bool hasVertexPosition();
	// bool hasVertexNormal();
	// bool hasVertexColor();
	bool hasVertexUV();

	// std::vector<FAMaterialComponent *>* getAvaliableComponents();
	// FAArmature *getArmature();

	~FAMesh();
	GLenum renderMode = GL_TRIANGLES;
	// std::vector<glm::mat4> animatedXForm;
	// std::vector<FABone *> bones;

	SharedPtr<Mesh> get_aiMesh() const { return _mesh; }; // PA2528-3 MEMORY FRAGMENTATION TEST CODE

private:
	void load(SharedPtr<Mesh> mesh);

protected:
	GLint numberOfVertices;

	GLuint VBO;
	GLuint EBO;
	GLuint VAO;

	SharedPtr<Mesh> _mesh;

};

#endif
