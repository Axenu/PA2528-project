#include <Game/FAMesh.h>
#include "gl/GLFunctions.h"

FAMesh::FAMesh() {
    //generate a default simple mesh with texture
	gl::CheckGLErrors("before FAMesh()");

	unsigned int indices[] = {
		0,2,1,
		2,3,1
	};
	this->numberOfVertices = 6;

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0, 0, 0,
		-0.5f, 0.5f, 0, 0, 1,
		0.5f, -0.5, 0, 1, 0,
		0.5f, 0.5f, 0, 1, 1
	};


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numberOfVertices * 3 * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numberOfVertices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	gl::CheckGLErrors("after FAMesh()");
}

FAMesh::FAMesh(std::string path) : FAMesh() {
	size_t place = path.find_last_of(".");
	// this->armature = nullptr;
    if (place != std::string::npos) {
        std::string filetype = path.substr(place+1, path.length());
        if (filetype == "fa"){
            // loadNewFAModel("/Users/Axenu/Developer/FireArrow-2.0/resources/models/" + path);
		} else if (filetype == "obj") {
			loadOBJModel("Resources/models/" + path);
		}
    } else {
        // loadFAModel("/Users/Axenu/Developer/FireArrow-2.0/resources/models/" + path);
    }
}

FAMesh::FAMesh(SharedPtr<Mesh> mesh) : _mesh(mesh) {
	load(**mesh->aiMesh);
}

void FAMesh::load(aiMesh &mesh) {
	std::vector<GLfloat> vertices;

	unsigned int *faceArray;
	faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh.mNumFaces * 3);
	unsigned int faceIndex = 0;

	for (unsigned int t = 0; t < mesh.mNumFaces; ++t) {
		aiFace face = mesh.mFaces[t];

		memcpy(&faceArray[faceIndex], face.mIndices, 3 * sizeof(unsigned int));
		faceIndex += 3;
	}

	if (!mesh.HasTextureCoords(0)) {
		std::cout << "mesh you are trying to load does not have any texture coordinates!!!" << std::endl;
		_hasUV = false;
	}

	for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
		vertices.push_back(mesh.mVertices[i].x);
		vertices.push_back(mesh.mVertices[i].y);
		vertices.push_back(mesh.mVertices[i].z);
		if (_hasUV) {
			vertices.push_back(mesh.mTextureCoords[0][i].x);
			vertices.push_back(mesh.mTextureCoords[0][i].y);
		}
	}
	this->numberOfVertices = mesh.mNumVertices;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.mNumFaces * 3, faceArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	int attributes = 3;
	if (_hasUV)
		attributes += 2;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributes * sizeof(GLfloat), (GLvoid *)(0 * sizeof(GLfloat)));
	if (_hasUV) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attributes * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

FAMesh::FAMesh(aiMesh &mesh) {
	load(mesh);
}

void FAMesh::loadOBJModel(std::string path) {
	std::ifstream file (path);
	std::vector<glm::vec3> vertexArray = std::vector<glm::vec3>();
	std::vector<glm::vec2> uvArray = std::vector<glm::vec2>();
	std::vector<glm::vec3> normalArray = std::vector<glm::vec3>();
	std::vector<glm::vec3> indicesArray;
	std::vector<glm::vec3> tengentArray;
	std::vector<glm::vec3> biTangentArray;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	if (!file.is_open()) {
		std::cout << "File " << path << "not found!" << std::endl;
		return;
	}
	int count = 0;
	std::string line;
	std::string option;
	while (!file.eof()) {
		option = "#";
		getline(file, line);
		std::stringstream ss(line);
		ss >> option;
		if (option == "#") {
//			std::cout << "comment: " << line << std::endl;
		} else if (option == "v") {
			glm::vec3 v;
			ss >> v.x >> v.y >> v.z;
			// std::cout << "vertex: " << line << std::endl;
			vertexArray.push_back(v);
		} else if (option == "vt") {
			glm::vec2 v;
			ss >> v.x >> v.y;
			// std::cout << "vertexUV: " << line << std::endl;
			uvArray.push_back(v);
		} else if (option == "vn") {
			glm::vec3 v;
			ss >> v.x >> v.y >> v.z;
			// std::cout << "normal: " << line << std::endl;
			normalArray.push_back(v);
		} else if (option == "f") {
			std::string index;

			for (int i = 0; i < 3; i++) {
				ss >> index;
//				std::cout << "index: " << index << std::endl;
				std::size_t d = index.find("/");
				int i1, i2, i3;
				i1 = atoi(index.substr(0, d).c_str()) - 1;
				index = index.substr(d+1);
				d = index.find("/");
				i2 = atoi(index.substr(0,d).c_str()) - 1;
				i3 = atoi(index.substr(d+1).c_str()) - 1;

				indicesArray.push_back((glm::vec3(i1,i2,i3)));


			}
		} else if (option == "mtllib") {

			objMaterailLib = "/Users/Axenu/Developer/FireArrow-2.0/resources/models/" + line.substr(7);

		} else if (option == "usemtl") {
			objMaterial = line.substr(7);
		} else {
//			std::cout << line << std::endl;
		}

		//calculate vertices
		for (int i = 0; i < (int)indicesArray.size(); i+=3) {

			//calculate tangent
			glm::vec3 index0 = indicesArray[i];
			glm::vec3 index1 = indicesArray[i + 1];
			glm::vec3 index2 = indicesArray[i + 2];

			// glm::vec3 dv1 = vertexArray[index1.x] - vertexArray[index0.x];//d
			// glm::vec3 dv2 = vertexArray[index2.x] - vertexArray[index0.x];//e
            //
			// glm::vec2 duv1 = uvArray[index1.y] - uvArray[index0.y];//f
			// glm::vec2 duv2 = uvArray[index2.y] - uvArray[index0.y];//g
            //
			// float r = 1.0/(duv1.x * duv2.y - duv1.y * duv2.x);
			// glm::vec3 tangent = (dv1 * duv2.y - dv2 * duv1.y) * r;
			// glm::vec3 bitangent = (dv2 * duv1.x - dv1 * duv2.x) * r;

			vertices.push_back(vertexArray[index0.x].x);
			vertices.push_back(vertexArray[index0.x].y);
			vertices.push_back(vertexArray[index0.x].z);
			// vertices.push_back(uvArray[index0.y].x);
			// vertices.push_back(uvArray[index0.y].y);
			// vertices.push_back(normalArray[index0.z].x);
			// vertices.push_back(normalArray[index0.z].y);
			// vertices.push_back(normalArray[index0.z].z);
			// vertices.push_back(tangent.x);
			// vertices.push_back(tangent.y);
			// vertices.push_back(tangent.z);
			// vertices.push_back(bitangent.x);
			// vertices.push_back(bitangent.y);
			// vertices.push_back(bitangent.z);
			indices.push_back(count);
			count++;

			vertices.push_back(vertexArray[index1.x].x);
			vertices.push_back(vertexArray[index1.x].y);
			vertices.push_back(vertexArray[index1.x].z);
			// vertices.push_back(uvArray[index1.y].x);
			// vertices.push_back(uvArray[index1.y].y);
			// vertices.push_back(normalArray[index1.z].x);
			// vertices.push_back(normalArray[index1.z].y);
			// vertices.push_back(normalArray[index1.z].z);
			// vertices.push_back(tangent.x);
			// vertices.push_back(tangent.y);
			// vertices.push_back(tangent.z);
			// vertices.push_back(bitangent.x);
			// vertices.push_back(bitangent.y);
			// vertices.push_back(bitangent.z);
			indices.push_back(count);
			count++;

			vertices.push_back(vertexArray[index2.x].x);
			vertices.push_back(vertexArray[index2.x].y);
			vertices.push_back(vertexArray[index2.x].z);
			// vertices.push_back(uvArray[index2.y].x);
			// vertices.push_back(uvArray[index2.y].y);
			// vertices.push_back(normalArray[index2.z].x);
			// vertices.push_back(normalArray[index2.z].y);
			// vertices.push_back(normalArray[index2.z].z);
			// vertices.push_back(tangent.x);
			// vertices.push_back(tangent.y);
			// vertices.push_back(tangent.z);
			// vertices.push_back(bitangent.x);
			// vertices.push_back(bitangent.y);
			// vertices.push_back(bitangent.z);
			indices.push_back(count);
			count++;
		}

	}

	this->numberOfVertices = count;
    //std::cout << count << std::endl;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	int attributes = 3 * sizeof(GLfloat);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) 0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) (3 *sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) (5 *sizeof(GLfloat)));
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) (8 *sizeof(GLfloat)));
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) (11 *sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void FAMesh::render() const {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(renderMode, numberOfVertices, GL_UNSIGNED_INT, NULL);
}

// bool FAMesh::hasVertexPosition() {
//     return this->_hasPosition;
// }

void FAMesh::update(float dt) {
	// if (this->_hasArmature) {
	// 	this->armature->update(dt);
	// }
}

// bool FAMesh::hasVertexNormal() {
//     return this->_hasNormal;
// }

// bool FAMesh::hasVertexColor() {
//     return this->_hasColor;
// }

// bool FAMesh::hasVertexUV() {
//     return this->_hasUV;
// }

// std::string FAMesh::getOBJMaterialLib() {
// 	return this->objMaterailLib;
// }
//
// std::string FAMesh::getOBJMaterial() {
// 	return this->objMaterial;
// }
//
// FAAABB FAMesh::getBounds() {
// 	return this->boundingBox;
// }
//
// std::vector<FAMaterialComponent *>* FAMesh::getAvaliableComponents() {
//     return &this->avaliableVertexComponents;
// }
//
// FAArmature *FAMesh::getArmature() {
// 	return this->armature;
// }

bool FAMesh::hasVertexUV()
{
	return _hasUV;
}

FAMesh::~FAMesh() {
	// delete armature;
	//delete gl resourcs
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}
