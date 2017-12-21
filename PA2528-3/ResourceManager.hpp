#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__

#include <atomic>

#include "gui_t.hpp"
#include "SharedPtr.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "HashMap.hpp"
#include "SpinLock.hpp"
#include "Promise.hpp"
#include "Function.hpp"
#include "Atomic.hpp"

#include "PA2528-2/MemoryTracker.h"

// Asynchronous operations are handled by the ResourceManager. Not the PackageReader.
class ResourceManager {
    public:
        static void initialize();

        // Synchronous load methods.
        static SharedPtr<Texture> loadTexture(gui_t gui);
        static SharedPtr<Mesh> loadMesh(gui_t gui);

        // Asynchronous load methods.
        // Something like this... Poll the handle to check availability.
        static Promise<SharedPtr<Texture>> aloadTexture(gui_t gui);
        static Promise<SharedPtr<Mesh>> aloadMesh(gui_t gui);

        // Something like this... The callback is invoked when operation completes.
        // The SharedPtr parameter of the callback contains the loaded resource.
        static void aloadTexture(gui_t gui, Function<void(SharedPtr<Texture>)> callback);
        static void aloadMesh(gui_t gui, Function<void(SharedPtr<Mesh>)> callback);

        static void garbageCollectTextures();
        static void garbageCollectMeshes();
        static void garbageCollect();

        static void setMemoryLimit(size_t limit);

		// Returns a float [0, 1] representing how close to the memory limit the resource manager is.
		static float getMemoryFillRatio();

    private:
		template<typename T>
		struct Entry {
			Entry(size_t s) : size(s) {}
			SpinLock lock;
			SharedPtr<T> data;
			size_t size;
			bool isLoading = false;
			Promise<SharedPtr<T>> promise;
		};

        static bool fitLimit(size_t loadSize);

		template<typename T>
		static SharedPtr<T> load(HashMap<gui_t, Entry<T>*>& map, gui_t gui);

		template<typename T>
		static Promise<SharedPtr<T>> aload(HashMap<gui_t, Entry<T>*>& map, gui_t gui);

		template<typename T>
		static SharedPtr<Promise<SharedPtr<T>>> getExistingPromise(HashMap<gui_t, Entry<T>*>& map, gui_t gui);

		template<typename T>
		static inline T* packageLoad(gui_t gui);

		template<typename T>
		static void garbageCollect(HashMap<gui_t, Entry<T>*>& map);

    private:
        
        static HashMap<gui_t, Entry<Texture>*> mTextures;
        static HashMap<gui_t, Entry<Mesh>*> mMeshes;
        static Atomic mSize;
        static size_t mSizeLimit;

};

#include "ResourceManager.inl"

#endif // __RESOURCE_MANAGER__
