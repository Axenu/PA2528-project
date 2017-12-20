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
	MemoryTracker::incrementResourceManagerCacheMisses();

	if (entry.promise != nullptr) {
		Promise<SharedPtr<T>> promise = *entry.promise;
		entry.lock.unlock();
		return promise.get();
	}

	std::cout << "loading " << gui << std::endl;
	SharedPtr<SharedPtr<SharedPtr<T>>> promise(new SharedPtr<SharedPtr<T>>(new SharedPtr<T>()));
	SharedPtr<bool> isReady(new bool());
	*isReady = false;
	Semaphore semaphore;
	entry.promise = new Promise<SharedPtr<T>>(promise, isReady, semaphore);

	entry.lock.unlock();

	if (!fitLimit(entry.size)) {
		std::cerr << "Failed to load resource (" << gui << "). Memory limit exceeded." << std::endl;
		// Loading this resource will violate the memory limit. Return default res<T>. TODO
		return nullptr;
	}

	res = packageLoad<T>(gui);
	assert(res != nullptr);
	if (res == nullptr) {
		// No such resource exists. Return default error res<T>. TODO
		return nullptr;
	}

	entry.lock.lock();
	assert(entry.data == nullptr);
	entry.data = res;
	*promise = new SharedPtr<T>(res);
	assert(entry.data.getReferenceCount() == 3);
	std::cout << "finished " << gui << std::endl;
	entry.lock.unlock();

	*isReady = true;
	semaphore.signal();
	mSize += entry.size;

	return res;
}

template<typename T>
static void ResourceManager::garbageCollect(HashMap<gui_t, Entry<T>*>& map) {
	for (Entry<T>* entry : map) {
		entry->lock.lock();
		if (entry->data != nullptr && entry->data.getReferenceCount() <= 2) {
			assert(entry->data.getReferenceCount() == 2);
			assert(entry->promise != nullptr);
			entry->data = nullptr;
			entry->promise = nullptr;
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
	SharedPtr<Promise<SharedPtr<T>>> promise = entry.promise;
	entry.lock.unlock();

	if (res == nullptr) {
		MemoryTracker::incrementResourceManagerCacheMisses();
	}
	else {
		MemoryTracker::incrementResourceManagerCacheHits();
	}

	return promise;
}

