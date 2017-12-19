#include <fstream>

#include "MemoryTracker.h"

size_t MemoryTracker::totalIndependentSize = 0;
std::unordered_map<size_t, std::string> MemoryTracker::IDtoName;
std::unordered_map<void*, MemoryTracker::Allocation> MemoryTracker::independentAllocations;
std::unordered_map<size_t, MemoryTracker::ReservedMemoryArea> MemoryTracker::reservedAreas;
std::unordered_map<void*, MemoryTracker::Allocation> MemoryTracker::dependentAllocations;
std::ostringstream MemoryTracker::outstream;
size_t MemoryTracker::resourceManagerCacheMisses = 0;
size_t MemoryTracker::resourceManagerCacheHits = 0;

void MemoryTracker::setAllocatorName(size_t ID, std::string name)
{
	IDtoName.insert(std::make_pair(ID, name));
}

void MemoryTracker::allocIndependent(void* p, size_t size, size_t ID)
{
	independentAllocations.insert(std::make_pair(p, Allocation(ID, size)));
	totalIndependentSize += size;
}

void MemoryTracker::deallocIndependent(void* p)
{
	totalIndependentSize -= independentAllocations[p].size;
	independentAllocations.erase(p);
}

void MemoryTracker::addReserved(void* p, size_t size, size_t ID)
{
	reservedAreas.insert(std::make_pair(ID, ReservedMemoryArea(p, size)));
}

void MemoryTracker::removeReserved(size_t ID)
{
	reservedAreas.erase(ID);
}

void MemoryTracker::allocDependent(void* p, size_t size, size_t ID)
{
	dependentAllocations.insert(std::make_pair(p, Allocation(ID, size)));

	reservedAreas[ID].amountUsed += size;
}

void MemoryTracker::deallocDependent(void* p)
{
	size_t ID = dependentAllocations[p].ID;
	reservedAreas[ID].amountUsed -= dependentAllocations[p].size;
	dependentAllocations.erase(p);
}

size_t MemoryTracker::getTotalMemoryUsage()
{
	size_t totalMem = totalIndependentSize;

	for (auto& area : reservedAreas)
		totalMem += area.second.size;

	return totalMem;
}

size_t MemoryTracker::getTotalWastedMemory()
{
	size_t wastedMem = 0;

	for (auto& area : reservedAreas)
		wastedMem += area.second.size - area.second.amountUsed;

	return wastedMem;
}

void MemoryTracker::writeLogToFile(std::string filename)
{
	std::ofstream f(filename);
	f << outstream.str();
	f.close();
}

std::vector<AllocatorInfo> MemoryTracker::getAllocatorsInfo()
{
	std::vector<AllocatorInfo> infos;

	for (auto& area : reservedAreas) {
		infos.push_back(AllocatorInfo(area.first, IDtoName[area.first], area.second.size, area.second.amountUsed));
	}

	return infos;
}

void MemoryTracker::incrementResourceManagerCacheMisses()
{
	resourceManagerCacheMisses++;
}

void MemoryTracker::incrementResourceManagerCacheHits()
{
	resourceManagerCacheHits++;
}

size_t MemoryTracker::getResourceManagerCacheMisses()
{
	return resourceManagerCacheMisses;
}

size_t MemoryTracker::getResourceManagerCacheHits()
{
	return resourceManagerCacheHits;
}
