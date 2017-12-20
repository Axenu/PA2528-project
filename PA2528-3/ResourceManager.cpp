#include "ResourceManager.hpp"
#include "PackageReader.hpp"
#include "ThreadPool.hpp"

HashMap<gui_t, ResourceManager::Entry<Texture>*> ResourceManager::mTextures;
HashMap<gui_t, ResourceManager::Entry<Mesh>*> ResourceManager::mMeshes;
Atomic ResourceManager::mSize = 0;
size_t ResourceManager::mSizeLimit = -1;

void ResourceManager::initialize() {
    static bool isInitialized = false;
    if(isInitialized) {
        return;
    }
    isInitialized = true;

    Array<PackageReader::MetaData> metaData = PackageReader::getMetaData();
    for(size_t i = 0; i < metaData.size; i++) {
        const PackageReader::MetaData& d = metaData.data[i];
        using T = PackageReader::MetaData::Type;
        switch(d.type) {
            case T::TEXTURE:    mTextures.insert(d.gui, new Entry<Texture>(d.size)); std::cout << "found GUID: " << d.gui << " type: Texture" << std::endl; break;
			case T::MESH:       mMeshes.insert(d.gui, new Entry<Mesh>(d.size)); std::cout << "found GUID: " << d.gui << " type: Mesh" << std::endl; break;
            default:            break;
        }
    }
    delete metaData.data;
}

SharedPtr<Texture> ResourceManager::loadTexture(gui_t gui) {
	return load(mTextures, gui);
}

SharedPtr<Mesh> ResourceManager::loadMesh(gui_t gui) {
	return load(mMeshes, gui);
}

Promise<SharedPtr<Texture>> ResourceManager::aloadTexture(gui_t gui) {
	SharedPtr<Promise<SharedPtr<Texture>>> promise = getExistingPromise(mTextures, gui);
	if (promise != nullptr) {
		return *promise;
	}
	return ThreadPool::promise<SharedPtr<Texture>>([gui](){return loadTexture(gui);});
}

Promise<SharedPtr<Mesh>> ResourceManager::aloadMesh(gui_t gui) {
	SharedPtr<Promise<SharedPtr<Mesh>>> promise = getExistingPromise(mMeshes, gui);
	if (promise != nullptr) {
		return *promise;
	}
	return ThreadPool::promise<SharedPtr<Mesh>>([gui]() {return loadMesh(gui);});
}


void ResourceManager::aloadTexture(gui_t gui, Function<void(SharedPtr<Texture>)> callback) {
    ThreadPool::launch([gui, callback](){callback(loadTexture(gui));});
}

void ResourceManager::aloadMesh(gui_t gui, Function<void(SharedPtr<Mesh>)> callback) {
    ThreadPool::launch([gui, callback](){callback(loadMesh(gui));});
}


void ResourceManager::garbageCollectTextures() {
	garbageCollect(mTextures);
}

void ResourceManager::garbageCollectMeshes() {
	garbageCollect(mMeshes);
}

void ResourceManager::garbageCollect() {
	garbageCollectMeshes();
	garbageCollectTextures();
}

void ResourceManager::setMemoryLimit(size_t limit) {
    mSizeLimit = limit;
}

float ResourceManager::getMemoryFillRatio()
{
	return mSize.load() / (float)mSizeLimit;
}

bool ResourceManager::fitLimit(size_t loadSize) {
    if((mSize.load() + loadSize) <= mSizeLimit) {
        return true;
    }

    garbageCollect();
    return (mSize.load() + loadSize) <= mSizeLimit;
}


template<>
inline Mesh* ResourceManager::packageLoad<Mesh>(gui_t gui) {
	return PackageReader::loadMesh(gui);
}

template<>
inline Texture* ResourceManager::packageLoad<Texture>(gui_t gui) {
	return PackageReader::loadTexture(gui);
}
