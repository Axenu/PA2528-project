#ifndef __First__FAModel__
#define __First__FAModel__

#include <iostream>
#include "FAMesh.h"
#include "FAMaterial.h"
#include "Camera.h"
#include "Node.h"

class FAModel : public FANode {

private:
	FAMesh *mesh;
	FAMaterial *material;
    Camera *cam;
public:
	FAModel();
	FAModel(FAMesh *mesh);
	FAModel(FAMesh *mesh, FAMaterial *material);

	void setMaterial(FAMaterial *material);
	void setMesh(FAMesh *mesh);
	void setMesh(std::string path);

	// void addMaterialComponent(FAMaterialComponent *component);

    void setCam(Camera *cam);

	FAMaterial &getMaterial();
	const FAMesh &getMesh() const;
	glm::mat4 &getModelMatrix();

	virtual void onUpdate(float dt);
    virtual void onRender();

	~FAModel();

};

#endif
