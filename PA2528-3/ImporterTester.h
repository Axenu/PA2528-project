#ifndef __ImporterTester__
#define __ImporterTester__

#include "ImporterManager.h"
#include "PackageReader.hpp"

// Simple tests to check if the Importer Manager are able to load a specific file.
// Can load from a file "path/file" or memory "memory buffer, length of buffer"

class ImporterTester
{
public:
	static void importerTest()
	{
		ImporterManager* importer = new ImporterManager();

		std::cerr << "Initializing Assimp using the import manager." << std::endl;
		importer->initLoader(ImporterManager::Importers::loader_assimp);
		
		std::cerr << "Deinitializing Assimp using the import manager." << std::endl;
		importer->destroyLoader(ImporterManager::Importers::loader_assimp);

		delete importer;
	}

	static void imageFileTest(std::string imagefile)
	{
		Texture* texture = new Texture();

		ImporterManager* importer = new ImporterManager();

		std::cerr << "Loading " << imagefile << " using stb_image." << std::endl;
		texture = importer->loadTextureFromFile(imagefile);

		std::cerr << "Unloading " << imagefile << " using stb_image." << std::endl;
		importer->freeTexture(texture);

		delete importer;
	}

	static void imageMemoryTest(void* buffer, int length, FileType fileType)
	{
		Texture* texture = new Texture();

		ImporterManager* importer = new ImporterManager();

		std::cerr << "Loading image from memory using stb_image." << std::endl;
		texture = importer->loadTextureFromMemory(buffer, length, fileType);

		std::cerr << "Unloading image from memory using stb_image." << std::endl;
		importer->freeTexture(texture);

		delete importer;
	}
	static void modelFileTest(std::string modelfile)
	{
		bool result;
		std::cerr << "Assimp Loading: " << modelfile << std::endl;
		AssimpLoader* loader = new AssimpLoader();
		result = loader->importFromFile(modelfile);

		if (result)
		{
			std::cerr << "Finished Loading: " << modelfile << std::endl;
		}
		else
		{
			std::cerr << "Failed Loading: " << modelfile << std::endl;
		}

		delete loader;
	}

	static Mesh* meshFileTest(std::string modelfile)
	{
		std::cerr << "Assimp Loading mesh from: " << modelfile << std::endl;
		AssimpLoader* loader = new AssimpLoader();
		Mesh* mesh = loader->loadMeshFromFile(modelfile);

		if (mesh)
		{
			std::cerr << "Successfully Loaded mesh: " << modelfile << std::endl;
		}
		else
		{
			std::cerr << "Failed to load mesh: " << modelfile << std::endl;
		}

		delete loader;

		return mesh;
	}

	static Mesh* meshMemoryTest(const void* buffer, size_t length, FileType fileType)
	{
		std::cerr << "Assimp Loading mesh from memory." << std::endl;
		AssimpLoader* loader = new AssimpLoader();
		Mesh* mesh = loader->loadMeshFromMemory(buffer, length, fileType);

		if (mesh)
		{
			std::cerr << "Successfully Loaded mesh from memory!" << std::endl;
		}
		else
		{
			std::cerr << "Failed to load mesh from memory!" << std::endl;
		}

		delete loader;

		return mesh;
	}
};

#endif
