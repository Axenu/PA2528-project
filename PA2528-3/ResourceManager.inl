template<typename T>
static SharedPtr<T> ResourceManager::load(HashMap<gui_t, Entry<T>*>& map, gui_t gui) {
	Entry<T>** entryPtr = map.find(gui);
	if (!entryPtr) {
		// No such resource exists. Return default error res<T>. TODO
		std::cout << "Failed to load rescource, not found!" << std::endl;
		return nullptr;
	}
	Entry<T>& entry = **entryPtr;

	entry.lock.lock();
	SharedPtr<T> res = entry.data;

	if (res != nullptr) {
		entry.lock.unlock();
		MemoryTracker::incrementResourceManagerCacheHits();
		return res;
	}

	if (entry.isLoading) {
		Promise<SharedPtr<T>> promise = entry.promise;
		entry.lock.unlock();
		return promise.get();
	}

	entry.isLoading = true;
	entry.lock.unlock();

	MemoryTracker::incrementResourceManagerCacheMisses();
	entry.promise.clear();

	std::cout << "loading " << gui << std::endl;
	if (!fitLimit(entry.size)) {
		std::cerr << "Failed to load resource (" << gui << "). Memory limit exceeded." << std::endl;
		// Loading this resource will violate the memory limit. Return default res<T>. TODO
		entry.lock.lock();
		entry.promise.fulfill(res);
		entry.isLoading = false;
		entry.lock.unlock();
		return res;
	}

	res = packageLoad<T>(gui);
	assert(res != nullptr);
	if (res == nullptr) {
		// No such resource exists. Return default error res<T>. TODO
		entry.lock.lock();
		entry.promise.fulfill(res);
		entry.isLoading = false;
		entry.lock.unlock();
		return res;
	}

	entry.lock.lock();
	assert(entry.data == nullptr);
	entry.data = res;
	entry.promise.fulfill(res);
	entry.isLoading = false;
	assert(entry.data.getReferenceCount() == 3);
	std::cout << "finished " << gui << std::endl;
	entry.lock.unlock();

	mSize += entry.size;

	return res;
}

template<typename T>
static Promise<SharedPtr<T>> ResourceManager::aload(HashMap<gui_t, Entry<T>*>& map, gui_t gui) {
	Entry<T>** entryPtr = map.find(gui);
	assert(entryPtr != nullptr);
	Entry<T>& entry = **entryPtr;

	entry.lock.lock();
	if (entry.data != nullptr || entry.isLoading) {
		Promise<SharedPtr<T>> promise = entry.promise;
		entry.lock.unlock();

		if (entry.data == nullptr) {
			MemoryTracker::incrementResourceManagerCacheMisses();
		}
		else {
			MemoryTracker::incrementResourceManagerCacheHits();
		}

		return promise;
	}

	entry.isLoading = true;
	entry.lock.unlock();

	entry.promise.clear();
	MemoryTracker::incrementResourceManagerCacheMisses();

	Atomic* size = &mSize;
	ThreadPool::launch([size, gui, entryPtr]() {
		Entry<T>& entry = **entryPtr;
		SharedPtr<T> res;
		if (!ResourceManager::fitLimit(entry.size)) {
			std::cerr << "Failed to load resource (" << gui << "). Memory limit exceeded." << std::endl;
			// Loading this resource will violate the memory limit. Return default res<T>. TODO
		}
		else {
			*size += entry.size;
			res = packageLoad<T>(gui);
			assert(res != nullptr);
			if (res == nullptr) {
				// No such resource exists. Return default error res<T>. TODO
			}
		}

		entry.lock.lock();
		entry.data = res;
		entry.promise.fulfill(res);
		entry.isLoading = false;
		entry.lock.unlock();

	});


	return entry.promise;
}

template<typename T>
static void ResourceManager::garbageCollect(HashMap<gui_t, Entry<T>*>& map) {
	for (Entry<T>* entry : map) {
		entry->lock.lock();
		if (entry->data != nullptr && entry->data.getReferenceCount() <= 2) {
			assert(entry->data.getReferenceCount() == 2);
			assert(entry->promise != nullptr);
			entry->data = nullptr;
			entry->promise.clear();
			mSize -= entry->size;
		}
		entry->lock.unlock();
	}
}

template<typename T>
SharedPtr<Promise<SharedPtr<T>>> ResourceManager::getExistingPromise(HashMap<gui_t, Entry<T>*>& map, gui_t gui) {
	Entry<T>** entryPtr = map.find(gui);
	assert(entryPtr != nullptr);
	Entry<T>& entry = **entryPtr;

	entry.lock.lock();
	SharedPtr<T> res = entry.data;
	SharedPtr<Promise<SharedPtr<T>>> promise = new Promise<SharedPtr<T>>(entry.promise);
	entry.lock.unlock();

	if (res != nullptr || ) {
		entry.lock.unlock();
		MemoryTracker::incrementResourceManagerCacheMisses();

	}

	if (res == nullptr) {
		MemoryTracker::incrementResourceManagerCacheMisses();
	}
	else {
		MemoryTracker::incrementResourceManagerCacheHits();
	}

	if(res == nullptr && )

	return promise;
}

