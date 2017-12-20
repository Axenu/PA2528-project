#ifndef __ASSIMPLOADER__
#define __ASSIMPLOADER__

#include <iostream>
#include <fstream>
#include <string>

#include "assimp\DefaultLogger.hpp"
#include "assimp\Importer.hpp"	// C++ importer interface
#include "assimp\scene.h"		// output data struct
#include "assimp\postprocess.h"	// post processing flags
#include "assimp\LogStream.hpp"

#include "Mesh.hpp"
#include "Texture.hpp"

enum FileType;

class AssimpLoader
{

public:
	AssimpLoader();
	~AssimpLoader();

	bool importFromFile(const std::string& file);
	bool importFromMemory(const void* buffer, size_t length);

	/*Mesh* loadMeshFromFile(const std::string& objFile);

	Mesh* loadMeshFromMemory(const void* buffer, size_t length, FileType fileType);*/

	/*Mesh* loadMeshFromFile(const std::string& objFile);

	Mesh* loadMeshFromMemory(const void* buffer, size_t length, FileType fileType);
	*/
	Mesh* loadMeshFromFile(const std::string& objFile);

	Mesh* loadMeshFromMemory(const void* buffer, size_t length, FileType fileType);

private:
	void createLogger();
	void destroyLogger();
	void logInfo(std::string log) // add message to file with "info" tag
	{
		Assimp::DefaultLogger::get()->info(log.c_str());
	}
	void logDebug(std::string log) // add message to file with "debug" tag
	{
		Assimp::DefaultLogger::get()->debug(log.c_str());
	}

	void renderScene(const aiScene* scene); // draw the scene

	Assimp::Importer m_importer;
	const aiScene* m_scene;
};

#endif