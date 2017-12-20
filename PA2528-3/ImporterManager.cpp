#include "ImporterManager.h"



ImporterManager::ImporterManager()
{

}


ImporterManager::~ImporterManager()
{
	
}

void ImporterManager::initLoader(int importer)
{
	switch (importer)
	{
	case loader_assimp:
		m_loaderAssimp = new AssimpLoader();
		break;

	case loader_stb_image:
		break;

	default:
		break;
	}
}

void ImporterManager::destroyLoader(int importer)
{
	if (m_loaderAssimp)
		delete m_loaderAssimp;
}

bool ImporterManager::importFromFile(int importer, std::string file)
{
	bool result;

	switch (importer)
	{
	case loader_assimp:
		result = m_loaderAssimp->importFromFile(file);
		break;

	case loader_stb_image:
		break;

	default:
		break;
	}

	return result;
}

bool ImporterManager::importFromMemory(int importer, const void* buffer, size_t length)
{
	bool result;

	switch (importer)
	{
	case loader_assimp:
		result = m_loaderAssimp->importFromMemory(buffer, length);
		break;

	case loader_stb_image:
		break;

	default:
		break;
	}

	return result;
}

Mesh* ImporterManager::loadMeshFromFile(std::string file)
{
	return m_loaderAssimp->loadMeshFromFile(file);
}

Mesh* ImporterManager::loadMeshFromMemory(const void* buffer, size_t length, FileType fileType)
{
	return m_loaderAssimp->loadMeshFromMemory(buffer, length, fileType);
}

Texture* ImporterManager::loadTextureFromFile(std::string file)
{
	return ImageLoader::loadFromFile(file.c_str());
}

Texture* ImporterManager::loadTextureFromMemory(void* buffer, int length, FileType fileType)
{
	return ImageLoader::loadFromMemory(buffer, length, fileType);
}

void ImporterManager::freeTexture(Texture* texture)
{
	if (texture)
	{
		ImageLoader::freeImage(texture->image);
		delete texture;
	}
}
