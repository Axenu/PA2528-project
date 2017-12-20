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
    Entry<Texture>** entryPtr = mTextures.find(gui);
    if(!entryPtr) {
        // No such resource exists. Return default error texture. TODO
        return nullptr;
    }
    Entry<Texture>& entry = **entryPtr;

    entry.lock.lock();
    SharedPtr<Texture> texture = entry.data;
    entry.lock.unlock();

    if(texture != nullptr) {
		MemoryTracker::incrementResourceManagerCacheHits();
        return texture;
    }
	MemoryTracker::incrementResourceManagerCacheMisses();

    if(!fitLimit(entry.size)) {
        std::cerr << "Failed to load resource (" << gui << "). Memory limit exceeded." << std::endl;
        // Loading this resource will violate the memory limit. Return default texture. TODO
        return nullptr;
    }

    texture = PackageReader::loadTexture(gui);
    if(texture == nullptr) {
        // No such resource exists. Return default error texture. TODO
        return nullptr;
    }

    entry.lock.lock();
    if(entry.data == nullptr) {
        entry.data = texture;
        mSize += entry.size;
    }
    else {
        texture = entry.data;
    }
    entry.lock.unlock();

    return texture;
}

SharedPtr<Mesh> ResourceManager::loadMesh(gui_t gui) {
    Entry<Mesh>** entryPtr = mMeshes.find(gui);
    if(!entryPtr) {
        // No such resource exists. Return default error mesh. TODO
		std::cout << "Failed to load rescource " << gui << ", not found!" << std::endl;
        return nullptr;
    }
    Entry<Mesh>& entry = **entryPtr;

    entry.lock.lock();
    SharedPtr<Mesh> mesh = entry.data;

    if(mesh != nullptr) {
		entry.lock.unlock();
		MemoryTracker::incrementResourceManagerCacheHits();
		return mesh;
    }
	MemoryTracker::incrementResourceManagerCacheMisses();

    if(!fitLimit(entry.size)) {
		entry.lock.unlock();
		std::cerr << "Failed to load resource (" << gui << "). Memory limit exceeded." << std::endl;
        // Loading this resource will violate the memory limit. Return default texture. TODO
        return nullptr;
    }

    mesh = PackageReader::loadMesh(gui);
	assert(mesh != nullptr);
    if(mesh == nullptr) {
        // No such resource exists. Return default error mesh. TODO
		entry.lock.unlock();
        return nullptr;
    }

    if(entry.data == nullptr) {
        entry.data = mesh;
        mSize += entry.size;
    }
    else {
        mesh = entry.data;
    }
    entry.lock.unlock();

    return mesh;
}

Promise<SharedPtr<Texture>> ResourceManager::aloadTexture(gui_t gui) {
    return ThreadPool::promise<SharedPtr<Texture>>([gui](){return loadTexture(gui);});
}

Promise<SharedPtr<Mesh>> ResourceManager::aloadMesh(gui_t gui) {
    return ThreadPool::promise<SharedPtr<Mesh>>([gui](){return loadMesh(gui);});
}


void ResourceManager::aloadTexture(gui_t gui, Function<void(SharedPtr<Texture>)> callback) {
    ThreadPool::launch([gui, callback](){callback(loadTexture(gui));});
}

void ResourceManager::aloadMesh(gui_t gui, Function<void(SharedPtr<Mesh>)> callback) {
    ThreadPool::launch([gui, callback](){callback(loadMesh(gui));});
}


void ResourceManager::garbageCollectTextures() {
    for(Entry<Texture>* entry : mTextures) {
        entry->lock.lock();
        if(entry->data.getReferenceCount() == 1) {
            entry->data = nullptr;
            mSize -= entry->size;
        }
        entry->lock.unlock();
    }
}

void ResourceManager::garbageCollectMeshes() {
    for(Entry<Mesh>* entry : mMeshes) {
        entry->lock.lock();
        if(entry->data.getReferenceCount() == 1) {
            entry->data = nullptr;
            mSize -= entry->size;
        }
        entry->lock.unlock();
    }
}

void ResourceManager::garbageCollect() {
    garbageCollectTextures();
    garbageCollectMeshes();
}

void ResourceManager::setMemoryLimit(size_t limit) {
    mSizeLimit = limit;
}

float ResourceManager::getMemoryFillRatio()
{
	return mSize.load() / (float) mSizeLimit;
}

bool ResourceManager::fitLimit(size_t loadSize) {
    if((mSize.load() + loadSize) <= mSizeLimit) {
        return true;
    }

    garbageCollect();
    return (mSize.load() + loadSize) <= mSizeLimit;
}
