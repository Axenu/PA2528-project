#ifndef __IMAGELOADER__
#define __IMAGELOADER__

#include <iostream>

#include "Texture.hpp"
#include "PackageReader.hpp"

class PackageReader;
enum FileType;

class ImageLoader
{
public:
	static Texture* loadFromFile(const char* file);

	static Texture* loadFromMemory(void* buffer, int length, FileType fileType);

	static void freeImage(unsigned char* image);
};

#endif