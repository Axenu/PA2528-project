#ifndef __PACKAGE_READER__
#define __PACKAGE_READER__

#include <string>
#include <fstream>
#include <Windows.h>

#include "Array.hpp"
#include "gui_t.hpp"
#include "OffsetPointer.hpp"
#include "ImporterManager.h"

#include "PA2528-2/PoolAllocator.h" // Pool allocator color depending on fragmentation

class Texture;
class Mesh;
enum FileType {
	OBJ, FBX, TGA, PNG, INVALID
};

// The PackageReader does not handle asynchronous loading.
// Its job is only to read a resource from a set package
// using a specified GUI.
// Note that the unbuffered I/O functionality belongs to
// the person responsible for developing the PackageReader.
class PackageReader {
    public:
        struct MetaData {
            enum class Type : char {
                TEXTURE,
                MESH,
                INVALID,
            };

			FileType fileType = INVALID;
            Type type = Type::INVALID;
            gui_t gui = 0;
			size_t offset;	// Number of bytes from the first file in the package to the start of this file.
			size_t size;	// Size of the file.
        };

    public:
        // Return true if package is found and valid. Else false.
        static bool setPackage(const char* path);

		// Clears the current package. Used when exiting application or switching package.
		static void clearPackage();

        // Return pointer to texture resource with specified GUI. Return nullptr if not found.
        static Texture* loadTexture(gui_t gui);

        // Return pointer to mesh resource with specified GUI. Return nullptr if not found.
        static Mesh* loadMesh(gui_t gui);

        // Return metadata of every resource in the package. Caller is responsible for the memory of returned array.
        static Array<MetaData> getMetaData();

		static std::string utf16ToUTF8(const std::wstring &s);

	private:
		// Opens and closes the package file.
		static bool openFile();
		static void closeFile();

		// Searches for and loads a file from the package into memory. Caller is responsible for the returned memory.
		// metaDataPos will contain index of the file in the metaData array.
		static OffsetPointer<void> loadFile(gui_t gui, size_t& metaDataPos);

		// Checks the sector size of the C drive
		static void findSectorSize();

		static std::wstring packagePath;
		static Array<MetaData> metaData;
		static std::ifstream file;

		// The location of the beginning of the first file.
		static size_t baseOffset;

		// Size of a sector on the C drive. Used for unbuffered reading.
		static long sectorSize;
};

#endif // __PACKAGE_READER__

