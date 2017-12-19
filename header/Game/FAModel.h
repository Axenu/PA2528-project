#ifndef __First__FAModel__
#define __First__FAModel__

#include <iostream>
#include "FAMesh.h"
#include "FAMaterial.h"
#include "Camera.h"
#include "Node.h"

#include "PA2528-3/SharedPtr.hpp"

class FAModel : public FANode {

private:
	SharedPtr<FAMesh> mesh;
	SharedPtr<FAMaterial> material;
public:
	FAModel();
	FAModel(SharedPtr<FAMesh> mesh);
	FAModel(SharedPtr<FAMesh> mesh, SharedPtr<FAMaterial> material);

	void setMaterial(SharedPtr<FAMaterial> material);
	void setMesh(SharedPtr<FAMesh> mesh);
	void setMesh(std::string path);

	FAMaterial &getMaterial();
	SharedPtr<FAMesh> getMesh() const;
	glm::mat4 &getModelMatrix();

	virtual void onUpdate(float dt);
	virtual void onRender(FrameData &fData);

	~FAModel();

};

#endif
