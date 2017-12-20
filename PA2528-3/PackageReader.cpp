#include <Windows.h>

#include "PackageReader.hpp"

#include "Texture.hpp"
#include "Mesh.hpp"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <locale.h>



std::wstring PackageReader::packagePath;
Array<PackageReader::MetaData> PackageReader::metaData;
std::ifstream PackageReader::file;
size_t PackageReader::baseOffset = 0;
long PackageReader::sectorSize = 0;

PoolAllocator* pool;
size_t poolElements = 8, poolBlockSize = 150000;

// Return true if package is found and valid. Else false.
bool PackageReader::setPackage(const char* path) {
	// Clear current package, if any.
	clearPackage();

	// Convert path to wchar
	const size_t cSize = strlen(path) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t outSize;
	mbstowcs_s(&outSize, wc, cSize, path, cSize-1);
	packagePath = wc;
	delete wc;

	// Open file
	openFile();

	baseOffset = 0;

	const int maxStringLength = 300;
	char* temp = new char[maxStringLength];

	file.getline(temp, maxStringLength);
	baseOffset += strlen(temp) + 1;
	metaData.size = strtoll(temp, NULL, 10);
	
	metaData.data = new PackageReader::MetaData[metaData.size];

	
	for (size_t i = 0; i < metaData.size; ++i) {
		// Read filename
		file.getline(temp, maxStringLength, ':');
		baseOffset += strlen(temp) + 1;

		// Read GUI
		file.getline(temp, maxStringLength, ':');
		baseOffset += strlen(temp) + 1;

		// Convert to bit representation and store in metadata array
		metaData.data[i].gui = strtoll(temp, NULL, 10);

		// Read file type
		file.getline(temp, maxStringLength, ':');
		baseOffset += strlen(temp) + 1;
		
		// Set file type
		if (strcmp(temp, "OBJ") == 0) {
			metaData.data[i].fileType = OBJ;
		}
		else if (strcmp(temp, "FBX") == 0) {
			metaData.data[i].fileType = FBX;
		}
		else if (strcmp(temp, "PNG") == 0) {
			metaData.data[i].fileType = PNG;
		}
		else if (strcmp(temp, "TGA") == 0) {
			metaData.data[i].fileType = TGA;
		}
		else {
			metaData.data[i].fileType = INVALID;
		}

		// Read resource type
		file.getline(temp, maxStringLength, ':');
		baseOffset += strlen(temp) + 1;

		// Set appropriate resource type.
		if (strcmp(temp, "M") == 0) {
			metaData.data[i].type = PackageReader::MetaData::Type::MESH;
		}
		else if (strcmp(temp, "T") == 0) {
			metaData.data[i].type = PackageReader::MetaData::Type::TEXTURE;
		}
		else {
			metaData.data[i].type = PackageReader::MetaData::Type::INVALID;
		}
		
		// Read size
		file.getline(temp, maxStringLength, ':');
		metaData.data[i].size = strtol(temp, NULL, 10);
		baseOffset += strlen(temp) + 1;

		// Read offset
		file.getline(temp, maxStringLength);
		metaData.data[i].offset = strtol(temp, NULL, 10);
		baseOffset += strlen(temp);

		++baseOffset; // newline
	}

	closeFile();
	delete[] temp;

	if (!sectorSize)
		findSectorSize();

	return true;
}

// Clears the current package. Used when exiting application or switching package.
void PackageReader::clearPackage() {
	delete[] metaData.data;
	metaData.data = nullptr;
	metaData.size = 0;
}

// Return pointer to texture resource with specified GUI. Return nullptr if not found.
Texture* PackageReader::loadTexture(gui_t gui) {
	size_t index = 0;
	OffsetPointer<void> mem = loadFile(gui, index);
    
	if (!mem.getPointer())
		return nullptr;

	// Read memory data into new texture (stb_image)
	ImporterManager* importer = new ImporterManager();
	Texture* texture = new Texture();	
	
	texture = importer->loadTextureFromMemory(mem.getPointer(), metaData.data[index].size, metaData.data[index].fileType);

	delete importer;

	return texture;	// return texture
}

// Return pointer to mesh resource with specified GUI. Return nullptr if not found.
Mesh* PackageReader::loadMesh(gui_t gui) {
	size_t index = 0;
	OffsetPointer<void> mem = loadFile(gui, index);

	if (!mem.getPointer())
		return nullptr;

	// Read memory data into new mesh (assimp)
	if (!pool)
	{
		pool = new PoolAllocator(poolBlockSize, poolElements);
		pool->setTrackingName("PoolAllocator Meshes");
	}

	ImporterManager* importer = new ImporterManager();
	importer->initLoader(0);

	Mesh* mesh = pool->alloc<Mesh>();
	mesh = importer->loadMeshFromMemory(mem.getPointer(), metaData.data[index].size, metaData.data[index].fileType);
	
	//Get memory allocated for the mesh
	//std::vector<AllocatorInfo> infos = MemoryTracker::getAllocatorsInfo();
	//mesh->memAllocated = poolBlockSize;
	// mesh->memAllocated = vertices * faces ... 

	mesh->memAllocated = poolBlockSize; // placeholder workaround

	// debug text
	std::cerr << "Size of mesh:       " << mesh->memUsed << std::endl;
	std::cerr << "Size of pool block: " << mesh->memAllocated << std::endl;


	delete importer;

	return mesh;	// return mesh
}

// Return array of all GUIs in the package. Caller is responsible for the memory of returned array.
Array<PackageReader::MetaData> PackageReader::getMetaData() {
	Array<MetaData> newArr;
	newArr.size = metaData.size;
	newArr.data = new PackageReader::MetaData[metaData.size];

	memcpy(newArr.data, metaData.data, sizeof(PackageReader::MetaData) * metaData.size);

    return newArr;
}

bool PackageReader::openFile()
{
	file.open(packagePath);
	if (!file.is_open()) {
		char* msg = new char[1000];
		std::cerr << "Error: " << strerror_s(msg, 1000, errno);
	}

	// Return the file's open status
	return file.is_open();
}

void PackageReader::closeFile()
{
	if (file.is_open())
		file.close();
}

// Searches for and loads a file from the package into memory. Caller is responsible for the returned memory.
// metaDataPos will contain index of the file in the metaData array.
OffsetPointer<void> PackageReader::loadFile(gui_t gui, size_t& metaDataPos)
{
	for (size_t i = 0; i < metaData.size; ++i) {
		if (metaData.data[i].gui == gui) {
			if (metaData.data[i].type == PackageReader::MetaData::Type::INVALID) {
				return OffsetPointer<void>();
			}

			// Return index in the metaData array
			metaDataPos = i;

			// Add sectorSize * 3, one for alignment in memory, one for alignment in the file, one for additional space read at the end of a sector
			void* rawMem = malloc(sectorSize * 3 + metaData.data[i].size);

			uintptr_t misalignedBytes = reinterpret_cast<uintptr_t>(rawMem) & (sectorSize - 1);
			unsigned int adjustment = sectorSize - misalignedBytes;

			long* alignedMem = reinterpret_cast<long*>(reinterpret_cast<uintptr_t>(rawMem) + adjustment);

			HANDLE fileHandle = CreateFile(packagePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
			HRESULT err = HRESULT_FROM_WIN32(GetLastError());

			LONG signedOffset = static_cast<long>(metaData.data[i].offset + baseOffset);
			// Find the closest previous sector in the file
			long offsetFromFileSector = signedOffset % sectorSize;

			SetFilePointer(fileHandle, signedOffset - offsetFromFileSector, NULL, FILE_BEGIN);
			err = HRESULT_FROM_WIN32(GetLastError());

			DWORD size = static_cast<unsigned long>(metaData.data[i].size);
			// Add enough space to read an even number of sectors
			size += sectorSize - (size % sectorSize);
			LPDWORD bytesRead = new DWORD;
			ReadFile(fileHandle, alignedMem, size, bytesRead, NULL);
			err = HRESULT_FROM_WIN32(GetLastError());
			CloseHandle(fileHandle);

			delete bytesRead;

			return OffsetPointer<void>(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(alignedMem) + static_cast<uintptr_t>(offsetFromFileSector)), adjustment + offsetFromFileSector);
		}
	}

	return OffsetPointer<void>();
}

void PackageReader::findSectorSize()
{
	// Get disk sector size
	STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR alignment = { 0 };
	WCHAR disk[] = L"\\\\.\\C:";


	DWORD Bytes = 0;
	STORAGE_PROPERTY_QUERY Query;

	ZeroMemory(&Query, sizeof(Query));

	HANDLE device = CreateFileW(disk, STANDARD_RIGHTS_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	Query.QueryType = PropertyStandardQuery;
	Query.PropertyId = StorageAccessAlignmentProperty;

	DeviceIoControl(device, IOCTL_STORAGE_QUERY_PROPERTY, &Query, sizeof(STORAGE_PROPERTY_QUERY), &alignment,
		sizeof(STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR), &Bytes, NULL);

	CloseHandle(device);

	sectorSize = alignment.BytesPerPhysicalSector;
}

std::string PackageReader::utf16ToUTF8(const std::wstring &s)
{
	const int size = ::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, NULL, 0, 0, NULL);

	std::vector<char> buf(size);
	::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, &buf[0], size, 0, NULL);

	return std::string(&buf[0]);
}