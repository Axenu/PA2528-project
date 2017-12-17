#ifndef __FAMATERIAL_COLOR__
#define __FAMATERIAL_COLOR__

#include "Game/FAMaterial.h"

class FAMaterialColor : public FAMaterial
{
private:
	GLint MemoryColor;
	float m_color[4] = { 0.0f,0.0f,0.0f,1.0f };

public:
	FAMaterialColor();
	FAMaterialColor(float color[4]);
	~FAMaterialColor();

	void bind(FrameData &fData);
	void setTexture(GLint texture);
	void setColor(float color[4]);
	void setColorMemFrag(float mem_total, float mem_used);
};

#endif