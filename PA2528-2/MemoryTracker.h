#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#ifndef MEMORYTRACKER_H
#define MEMORYTRACKER_H

// Contains info on the current state of an allocator
struct AllocatorInfo {
	size_t allocatorID;
	std::string allocatorName;
	size_t totalReserved;
	size_t totalUsed;

	AllocatorInfo(size_t ID, std::string name, size_t reserved, size_t used) : 
		allocatorID{ ID }, allocatorName{ name }, totalReserved { reserved }, totalUsed{ used }
	{}
};

class MemoryTracker {
public:
	// Sets a name for a specific allocator ID
	static void setAllocatorName(size_t ID, std::string name);

	// Track allocations or deallocations of memory not pre-reserved by an allocator
	static void allocIndependent(void* p, size_t size, size_t ID);
	static void deallocIndependent(void* p);

	// Adds or removes tracking of a block of memory reserved by an allocator
	// Only one reserved memory block per allocator
	static void addReserved(void* p, size_t size, size_t ID);
	static void removeReserved(size_t ID);

	// Track allocations or deallocations within a memory block reserved by an allocator
	static void allocDependent(void* p, size_t size, size_t ID);
	static void deallocDependent(void* p);

	// Allows writing to allocation log
	static std::ostringstream& out() { return outstream; }

	static size_t getTotalMemoryUsage();
	static size_t getTotalWastedMemory();
	static void writeLogToFile(std::string filename);

	static std::vector<AllocatorInfo> getAllocatorsInfo();

	static void incrementResourceManagerCacheMisses();
	static void incrementResourceManagerCacheHits();
	static size_t getResourceManagerCacheMisses();
	static size_t getResourceManagerCacheHits();

	static void addVRAM(size_t VRAM);
	static void removeVRAM(size_t VRAM);
	static size_t getVRAM();

private:
	// Holds info on an allocation
	struct Allocation {
		size_t ID;
		size_t size;

		Allocation(size_t ID, size_t size) {
			this->ID = ID;
			this->size = size;
		}

		Allocation() {}
	};

	// Holds info on a memory area reserved by an allocator
	struct ReservedMemoryArea {
		void* p;
		size_t size;
		size_t amountUsed;

		ReservedMemoryArea(void* p, size_t size) {
			this->p = p;
			this->size = size;
			amountUsed = 0;
		}
		ReservedMemoryArea() {}
	};

	static std::unordered_map<size_t, std::string> IDtoName;

	static std::unordered_map<void*, Allocation> independentAllocations;
	static size_t totalIndependentSize; // Total size of allocations in independentAllocations

	static std::unordered_map<size_t, ReservedMemoryArea> reservedAreas; // ID to ReservedMemoryArea
	static std::unordered_map<void*, Allocation> dependentAllocations;

	// Holds allocation log
	static std::ostringstream outstream;

	static size_t resourceManagerCacheMisses;
	static size_t resourceManagerCacheHits;

	static size_t VRAM;

	size_t getMemoryUsage(size_t ID);
	size_t getWasterMemory(size_t ID);
};

#endif