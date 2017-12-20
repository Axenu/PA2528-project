#ifndef __TEXTURE__
#define __TEXTURE__

class Texture { // or struct - doesn't matter;
public:
	unsigned char* image;
	int width;
	int height;
	int composition;
};

#endif // __TEXTURE__

