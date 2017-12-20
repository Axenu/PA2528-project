#ifndef __MESH__
#define __MESH__

#include "assimp\mesh.h"
#include <vector>

class Mesh { // or struct - doesn't matter
public:
	unsigned int *faceArray;
	unsigned int numFaces;

	std::vector<float> vertices;
	unsigned int numVertices;

	aiVector3D** textureCoords;
	unsigned int numTextureCoords;
	bool hasTextureCoords;

	size_t memUsed, memAllocated;
};
#endif // __MESH__