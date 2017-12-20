#ifndef __IMPORTERMANAGER__
#define __IMPORTERMANAGER__

#include "AssimpLoader.h"
#include "ImageLoader.h"
#include "PackageReader.hpp"

class PackageReader;
enum FileType;

// Keeps track of the different importers
// InitLoader(ImporterManager::Importers::loader_assimp) before loading meshes and models
// Stb_iamge does not need to be initialized or destroyed before use (loadTexture)
class ImporterManager
{
public:
	enum Importers { // not neeeded?
		loader_assimp = 0,
		loader_stb_image = 1
	};

public:
	ImporterManager();
	~ImporterManager();

	void initLoader(int importer);
	void destroyLoader(int importer);

	bool importFromFile(int importer, std::string file);
	bool importFromMemory(int importer, const void* buffer, size_t length);

	// assimp
	Mesh* loadMeshFromFile(std::string file);
	Mesh* loadMeshFromMemory(const void* buffer, size_t length, FileType fileType);
	
	// stb_image
	Texture* loadTextureFromFile(std::string file);
	Texture* loadTextureFromMemory(void* buffer, int length, FileType fileType);
	
	void freeTexture(Texture* texture);

private:
	//bool importUsingAssimp(std::string file);
	// importUsing2ndLibrary ...

	AssimpLoader* m_loaderAssimp;
};

#endif