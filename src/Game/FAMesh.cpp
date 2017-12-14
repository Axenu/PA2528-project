#include <Game/FAMesh.h>

FAMesh::FAMesh() {
    // this->_hasPosition = true;
    // this->_hasNormal = false;
    // this->_hasColor = false;
    // this->_hasUV = false;
    // this->_hasArmature = false;
	// this->_hasWeights = false;
	// this->armature = nullptr;
    // this->animatedXForm = std::vector<glm::mat4>();
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

FAMesh::FAMesh(aiMesh &mesh) {

	std::vector<GLfloat> vertices;

	unsigned int *faceArray;
	faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh.mNumFaces * 3);
	unsigned int faceIndex = 0;

	for (unsigned int t = 0; t < mesh.mNumFaces; ++t) {
		aiFace face = mesh.mFaces[t];

		memcpy(&faceArray[faceIndex], face.mIndices, 3 * sizeof(unsigned int));
		faceIndex += 3;
	}

	for (int i = 0; i < mesh.mNumVertices; i++) {
		vertices.push_back(mesh.mVertices[i].x);
		vertices.push_back(mesh.mVertices[i].y);
		vertices.push_back(mesh.mVertices[i].z);
		vertices.push_back(mesh.mTextureCoords[i]->x);
		vertices.push_back(mesh.mTextureCoords[i]->y);
	}

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

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

// FAMesh::FAMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, bool hasNormal, bool hasColor) : FAMesh() {

    // this->_hasColor = hasColor;
    // this->_hasNormal = hasNormal;
    //
    // if (hasColor) {
    //     avaliableVertexComponents.push_back(new FAVertexColorComponent());
    // }
    // if (hasNormal) {
    //     avaliableVertexComponents.push_back(new FAVertexNormalComponent());
    // }
    // // this->_hasPosition = true;
    //
    // // load model to graphicscard
    // this->numberOfVertices = (GLint) indices.size();
    //
    // //test
    // // GLfloat vertA[] = {0,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,1,0, 1,0,0, 1,0,0, 0,1,0, 1,0,0, 1,0,0};
    // // GLuint indA[] = {0,1,2,0,2,3};
    // // _hasColor = true;
    // // _hasNormal = true;
    //
    // glGenBuffers(1, &VBO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    //
    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //
    // glGenVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //
    // int attributes = 3;
    // if (_hasNormal) attributes +=3;
    // if (_hasColor) attributes +=3;
    //
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributes * sizeof(GLfloat), (GLvoid *) (0 * sizeof(GLfloat)));
    // int offset = 3;
    // if (_hasNormal) {
    //     glEnableVertexAttribArray(1);
    //     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attributes * sizeof(GLfloat), (GLvoid *) (offset * sizeof(GLfloat)));
    //     offset += 3;
    // }
    // if (_hasColor) {
    //     glEnableVertexAttribArray(2);
    //     glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, attributes * sizeof(GLfloat), (GLvoid *) (offset * sizeof(GLfloat)));
    // }
    //
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
// }

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
		for (int i = 0; i < indicesArray.size(); i+=3) {

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
    std::cout << count << std::endl;

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

	int attributes = 14 * sizeof(GLfloat);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) (3 *sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) (5 *sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) (8 *sizeof(GLfloat)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) (11 *sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
//
// void FAMesh::loadNewFAModel(std::string path) {
//     std::ifstream file (path);
//     std::vector<glm::vec3> vertexArray = std::vector<glm::vec3>();
//     std::vector<glm::vec3> normalArray = std::vector<glm::vec3>();
//     std::vector<glm::vec4> weightArray = std::vector<glm::vec4>();
//     std::vector<glm::vec4> boneGroupArray = std::vector<glm::vec4>();
//     // std::vector<glm::vec2> UVArray;
//     // std::vector<int> colorArray;
//     std::vector<glm::vec3> materialArray;
//     std::vector<GLfloat> vertices;
//     std::vector<GLuint> indices;
//
//     if (file.is_open()) {
//         while (!file.eof()) {
//             int count;
// 			std::string key;
// 			file >> key;
// //			std::cout << key << std::endl;
// //			key = "";
//             if (key == "v") {
//                 avaliableVertexComponents.push_back(new FAVertexPositionComponent());
//                 file >> count;
//                 // vertexArray = std::vector<glm::vec3>();
//                 glm::vec3 vec;
//                 for (int i = 0; i < count; i++) {
//                     file >> vec.x >> vec.y >> vec.z;
//                     vertexArray.push_back(vec);
//                 }
//             } else if (key == "n") {
//                 avaliableVertexComponents.push_back(new FAVertexNormalComponent());
//                 _hasNormal  = true;
//                 file >> count;
//                 glm::vec3 normal;
//                 for (int i = 0; i < count; i++) {
//                     file >> normal.x >> normal.y >> normal.z;
//                     normalArray.push_back(normal);
//                 }
//                 //add avaliable material component normals
//             } else if (key == "c") {
//                 avaliableVertexComponents.push_back(new FAVertexColorComponent());
//                 _hasColor = true;
//                 file >> count;
//                 glm::vec3 color;
//                 for (int i = 0; i < count; i++) {
//                     file >> color.x >> color.y >> color.z;
//                     materialArray.push_back(color);
//                 }
//                  //add avaliable material component colors
//             } else if (key == "w") {
//                 _hasWeights = true;
//                 file >> count;
//                 glm::vec4 index;
//                 glm::vec4 weights;
//                 for (int i = 0; i < count; i++) {
//                     file >> index.x >> weights.x >> index.y >> weights.y >> index.z >> weights.z >> index.w >> weights.w;
//                     weightArray.push_back(weights);
//                     boneGroupArray.push_back(index);
//                 }
//                  //add avaliable material component colors
//             } else if (key == "i") {
//                 file >> count;
//
//                 this->numberOfVertices = count * 3;
//                 for (int i = 0; i < this->numberOfVertices; i++) {
//                     indices.push_back(i);
//                 }
//
//                 int v0, v1, v2, n0 = 0, n1 = 0, n2 = 0, c = 0;
//
//                 for (int i = 0; i < count; i++) {
//                     file >> v0 >> v1 >> v2;
//                     if (_hasNormal) {
//                         file >> n0 >> n1 >> n2;
//                     }
//                     if (_hasColor) {
//                         file >> c;
//                     }
//                     glm::vec3 color = materialArray[c];
//
//                     vertices.push_back(vertexArray[v0].x);
//                     vertices.push_back(vertexArray[v0].y);
//                     vertices.push_back(vertexArray[v0].z);
//                     if (_hasNormal) {
//                         vertices.push_back(normalArray[n0].x);
//                         vertices.push_back(normalArray[n0].y);
//                         vertices.push_back(normalArray[n0].z);
//                     }
//                     if (_hasColor) {
//                         vertices.push_back(color.x);
//                         vertices.push_back(color.y);
//                         vertices.push_back(color.z);
//                     }
//                     if (_hasWeights) {
//                         vertices.push_back(boneGroupArray[v0].x);
//                         vertices.push_back(boneGroupArray[v0].y);
//                         vertices.push_back(boneGroupArray[v0].z);
//                         vertices.push_back(boneGroupArray[v0].w);
//                         vertices.push_back(weightArray[v0].x);
//                         vertices.push_back(weightArray[v0].y);
//                         vertices.push_back(weightArray[v0].z);
//                         vertices.push_back(weightArray[v0].w);
//                     }
//
//                     vertices.push_back(vertexArray[v1].x);
//                     vertices.push_back(vertexArray[v1].y);
//                     vertices.push_back(vertexArray[v1].z);
//                     if (_hasNormal) {
//                         vertices.push_back(normalArray[n1].x);
//                         vertices.push_back(normalArray[n1].y);
//                         vertices.push_back(normalArray[n1].z);
//                     }
//                     if (_hasColor) {
//                         vertices.push_back(color.x);
//                         vertices.push_back(color.y);
//                         vertices.push_back(color.z);
//                     }
//                     if (_hasWeights) {
//                         vertices.push_back(boneGroupArray[v1].x);
//                         vertices.push_back(boneGroupArray[v1].y);
//                         vertices.push_back(boneGroupArray[v1].z);
//                         vertices.push_back(boneGroupArray[v1].w);
//                         vertices.push_back(weightArray[v1].x);
//                         vertices.push_back(weightArray[v1].y);
//                         vertices.push_back(weightArray[v1].z);
//                         vertices.push_back(weightArray[v1].w);
//                     }
//
//                     vertices.push_back(vertexArray[v2].x);
//                     vertices.push_back(vertexArray[v2].y);
//                     vertices.push_back(vertexArray[v2].z);
//                     if (_hasNormal) {
//                         vertices.push_back(normalArray[n2].x);
//                         vertices.push_back(normalArray[n2].y);
//                         vertices.push_back(normalArray[n2].z);
//                     }
//                     if (_hasColor) {
//                         vertices.push_back(color.x);
//                         vertices.push_back(color.y);
//                         vertices.push_back(color.z);
//                     }
//                     if (_hasWeights) {
//                         vertices.push_back(boneGroupArray[v2].x);
//                         vertices.push_back(boneGroupArray[v2].y);
//                         vertices.push_back(boneGroupArray[v2].z);
//                         vertices.push_back(boneGroupArray[v2].w);
//                         vertices.push_back(weightArray[v2].x);
//                         vertices.push_back(weightArray[v2].y);
//                         vertices.push_back(weightArray[v2].z);
//                         vertices.push_back(weightArray[v2].w);
//                     }
//                 }
//             } else if (key == "b") {
//                 this->_hasArmature = true;
//                 this->armature = new FAArmature();
//                 file >> count;
//                 FABone *b = new FABone();
//                 for (int i = 0; i < count; i++) {
// 					b = new FABone();
//                     int parent;
//                     std::string name;
//                     glm::vec3 position;
//                     file >> name;
//                     file >> parent;
//                     file >> position.x >> position.y >> position.z;
// //                    position = glm::vec3(position.x, position.y, position.z);
//                     b->setName(name);
//                     b->setGlobalPosition(position);
//                     if (parent != -1) {
// 						b->setParent(armature->getBone(parent));
//                     } else {
//                         // rootBone = b;
//                         armature->setRootBone(b);
//                         b->setParent(NULL);
//                     }
//                     b->setScale(glm::vec3(1,1,1));
//                     b->setRotation(glm::vec3(0,0,0));
//                     armature->addBone(b);
//                     // bones.push_back(b);
//                 }
//                 armature->adjustPositions(armature->getRootBone(), glm::vec3());
//                 armature->setupBoneMatrices(armature->getRootBone());
//                 FASkinningVertexComponent *skinningComponent = new FASkinningVertexComponent();
//                 skinningComponent->setBonesArray((GLsizei) armature->getNumberOfBones(), armature->getAnimatedXForm());
//                 this->avaliableVertexComponents.push_back(skinningComponent);
//             } else if (key == "a") {
//                 file >> count;
//                 glm::vec4 quat;
//                 glm::quat q;
// 				FAAnimation *animation;
// 				FAFrame *frame;
// 				int c;
//                 for (int i = 0; i < count; i++) {
// 					file >> c;
// 					animation = new FAAnimation(c);
//                     for (int j = 0; j < c; j++) {
// 						frame = new FAFrame(armature->getNumberOfBones());
//                         for (int k = 0; k < armature->getNumberOfBones(); k++) {
//                             file >> q.y >> q.z >> q.x >> q.w;
// //							q.z *= -1;
// //                            q = glm::quat(quat.x, quat.z, quat.y, quat.w);
// 							frame->addQuaternion(q);
//                         }
// 						animation->addFrame(frame);
//                     }
// 					armature->addAnimation(animation);
//                 }
// 			} else {
// //				std::cout << key << std::endl;
// 			}
//         }
//         glGenBuffers(1, &VBO);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//         glGenBuffers(1, &EBO);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//         glGenVertexArrays(1, &VAO);
//         glBindVertexArray(VAO);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//         int attributes = 3 * sizeof(GLfloat);
//         if (_hasNormal) attributes +=3 * sizeof(GLfloat);
//         if (_hasColor) attributes +=3 * sizeof(GLfloat);
//         if (_hasUV) attributes +=2 * sizeof(GLfloat);
//         if (_hasArmature) attributes +=8 * sizeof(GLfloat);
//
//         int attribA = 1;
//
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) 0);
//         int offset = 3 * sizeof(GLfloat);
//         if (_hasNormal) {
//             glEnableVertexAttribArray(attribA);
//             glVertexAttribPointer(attribA, 3, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) offset);
//             offset += 3 * sizeof(GLfloat);
//             attribA++;
//         }
//         if (_hasColor) {
//             glEnableVertexAttribArray(attribA);
//             glVertexAttribPointer(attribA, 3, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) offset);
//             offset += 3 * sizeof(GLfloat);
//             attribA++;
//         }
//         if (_hasUV) {
//             glEnableVertexAttribArray(attribA);
//             glVertexAttribPointer(attribA, 2, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) offset);
//             offset += 2 * sizeof(GLfloat);
//             attribA++;
//         }
//         if (_hasArmature) {
//             glEnableVertexAttribArray(attribA);
//             glVertexAttribPointer(attribA, 4, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) offset);
//             offset += 4 * sizeof(GLfloat);
//             attribA++;
//             glEnableVertexAttribArray(attribA);
//             glVertexAttribPointer(attribA, 4, GL_FLOAT, GL_FALSE, attributes, (GLvoid *) offset);
//             offset += 4 * sizeof(GLfloat);
//         }
//
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//         glBindVertexArray(0);
//
// 		//calculate AABB
// 		glm::vec3 min = glm::vec3(0);
// 		glm::vec3 max = glm::vec3(0);
//
// 		for (int i = 0; i < vertexArray.size(); i++) {
// 			glm::vec3 v = vertexArray[i];
// 			if (v.x < min.x) {
// 				min.x = v.x;
// 			}
// 			if (v.x > max.x) {
// 				max.x = v.x;
// 			}
// 			if (v.y < min.y) {
// 				min.y = v.y;
// 			}
// 			if (v.y > max.y) {
// 				max.y = v.y;
// 			}
// 			if (v.z < min.z) {
// 				min.z = v.z;
// 			}
// 			if (v.z > max.z) {
// 				max.z = v.z;
// 			}
// 		}
//
// //		this->center = (min+max)*0.5f;
// //		this->globalCenter = this->center;
// //		this->half = max - this->center;
// 		boundingBox.center = (min+max)*0.5f;
// 		boundingBox.half = max - boundingBox.center;
// 		std::cout << boundingBox.center.x << std::endl;
// //		boundingBox = FAAABB((min+max)*0.5f, max - (min+max)*0.5f);
//
//     } else {
//          std::cout << "FAModel failed to load model: " << path << std::endl;
//     }
//          // std::cout << "FAModel finished loading: " << path << std::endl;
// }
//
// void FAMesh::loadFAModel(std::string path) {
//     srand (time(NULL));
//     std::ifstream file (path);
//     std::vector<glm::vec3> vertexArray;
//     std::vector<glm::vec3> normalArray;
//     std::vector<glm::vec2> UVArray;
//     std::vector<int> colorArray;
//     std::vector<glm::vec3> materialArray;
//     std::vector<GLfloat> vertices;
//     std::vector<GLuint> indices;
//
//     if (file.is_open() && file.is_open()) {
//
//         while (!file.eof()) {
//
//             std::string f;
//             int count;
//             file >> f;
//             if (f == "v") {
//                 avaliableVertexComponents.push_back(new FAVertexPositionComponent());
//                 file >> count;
//                 vertexArray = std::vector<glm::vec3>();
//                 glm::vec3 vec;
//                 for (int i = 0; i < count; i++) {
//                     file >> vec.x >> vec.y >> vec.z;
//                     vertexArray.push_back(vec);
//                 }
//             } else if (f == "n") {
//                 avaliableVertexComponents.push_back(new FAVertexNormalComponent());
//                 _hasNormal  = true;
//                 file >> count;
//                 normalArray = std::vector<glm::vec3> ();
//                 glm::vec3 normal;
//                 for (int i = 0; i < count; i++) {
//                     file >> normal.x >> normal.y >> normal.z;
//                     normalArray.push_back(normal);
//                 }
//             } else if (f == "c") {
//                 avaliableVertexComponents.push_back(new FAVertexColorComponent());
//                 _hasColor = true;
//                 file >> count;
//                 int index;
//                 for (int i = 0; i < count; i++) {
//                     file >> index;
//                     colorArray.push_back(index);
//                 }
//             } else if (f == "m") {
//                 file >> count;
//                 glm::vec3 color;
//                 for (int i = 0; i < count; i++) {
//                     file >> color.x >> color.y >> color.z;
//                     materialArray.push_back(color);
//                 }
//             } else if (f == "uv") {
//                 _hasUV = true;
//                 file >> count;
//                 glm::vec2 uv;
//                 for (int i = 0; i < count; i++) {
//                     file >> uv.x >> uv.y;
//                     UVArray.push_back(uv);
//                 }
//             } else if (f == "i") {
//                 file >> count;
//                 indices = std::vector<GLuint> ();
//                 vertices = std::vector<GLfloat> ();
//                 for (int i = 0; i < count * 3; i++) {
//                     indices.push_back(i);
//                 }
//                 float v1, v2, v3, n = 0, uv1 = 0, uv2 = 0, uv3 = 0;
//                 for (int i = 0; i < count; i++) {
//                     file >> v1 >> v2 >> v3;
//                     if (_hasNormal) {
//                         file >> n;
//                     }
//                     int face = 0;
//                     if (_hasColor) {
//                         face = colorArray[n];
//                     }
//                     if (_hasUV) {
//                         file >> uv1 >> uv2 >> uv3;
//                     }
//
//                     glm::vec3 color;
//                     if (_hasColor) {
//                         color = materialArray[face];
//                     }
//
//                     vertices.push_back(vertexArray[v1].x);
//                     vertices.push_back(vertexArray[v1].y);
//                     vertices.push_back(vertexArray[v1].z);
//                     if (_hasNormal) {
//                         vertices.push_back(normalArray[n].x);
//                         vertices.push_back(normalArray[n].y);
//                         vertices.push_back(normalArray[n].z);
//                     }
//                     if (_hasColor) {
//                         vertices.push_back(color.x);
//                         vertices.push_back(color.y);
//                         vertices.push_back(color.z);
//                     }
//                     if (_hasUV) {
//                         vertices.push_back(UVArray[uv1].x);
//                         vertices.push_back(UVArray[uv1].y);
//                     }
//
//                     vertices.push_back(vertexArray[v2].x);
//                     vertices.push_back(vertexArray[v2].y);
//                     vertices.push_back(vertexArray[v2].z);
//                     if (_hasNormal) {
//                         vertices.push_back(normalArray[n].x);
//                         vertices.push_back(normalArray[n].y);
//                         vertices.push_back(normalArray[n].z);
//                     }
//                     if (_hasColor) {
//                         vertices.push_back(color.x);
//                         vertices.push_back(color.y);
//                         vertices.push_back(color.z);
//                     }
//                     if (_hasUV) {
//                         vertices.push_back(UVArray[uv2].x);
//                         vertices.push_back(UVArray[uv2].y);
//                     }
//
//                     vertices.push_back(vertexArray[v3].x);
//                     vertices.push_back(vertexArray[v3].y);
//                     vertices.push_back(vertexArray[v3].z);
//                     if (_hasNormal) {
//                         vertices.push_back(normalArray[n].x);
//                         vertices.push_back(normalArray[n].y);
//                         vertices.push_back(normalArray[n].z);
//                     }
//                     if (_hasColor) {
//                         vertices.push_back(color.x);
//                         vertices.push_back(color.y);
//                         vertices.push_back(color.z);
//                     }
//                     if (_hasUV) {
//                         vertices.push_back(UVArray[uv3].x);
//                         vertices.push_back(UVArray[uv3].y);
//                     }
//                 }
//             } else {
//                 // std::cout << "Unknown character! asuming it's a comment: \"" << f << "\"" << std::endl;
//             }
//         }
//
//         // load model to graphicscard
//         this->numberOfVertices = indices.size();
//
//         //test
//         // GLfloat vertA[] = {0,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,1,0, 1,0,0, 1,0,0, 0,1,0, 1,0,0, 1,0,0};
//         // GLuint indA[] = {0,1,2,0,2,3};
//         // _hasColor = true;
//         // _hasNormal = true;
//
//         glGenBuffers(1, &VBO);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//         glGenBuffers(1, &EBO);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//         glGenVertexArrays(1, &VAO);
//         glBindVertexArray(VAO);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//         int attributes = 3;
//         if (_hasNormal) attributes +=3;
//         if (_hasColor) attributes +=3;
//         if (_hasUV) attributes +=2;
//
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributes * sizeof(GLfloat), (GLvoid *) (0 * sizeof(GLfloat)));
//         int offset = 3;
//         if (_hasNormal) {
//             glEnableVertexAttribArray(1);
//             glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attributes * sizeof(GLfloat), (GLvoid *) (offset * sizeof(GLfloat)));
//             offset += 3;
//         }
//         if (_hasColor) {
//             glEnableVertexAttribArray(2);
//             glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, attributes * sizeof(GLfloat), (GLvoid *) (offset * sizeof(GLfloat)));
//             offset += 3;
//         }
//         if (_hasUV) {
//             glEnableVertexAttribArray(3);
//             glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, attributes * sizeof(GLfloat), (GLvoid *) (offset * sizeof(GLfloat)));
//             offset += 2;
//         }
//
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//         glBindVertexArray(0);
//     } else {
//         std::cout << "FAModel failed to load model: " << path << std::endl;
//     }
// }

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

FAMesh::~FAMesh() {
	// delete armature;
	//delete gl resourcs
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}
