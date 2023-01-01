#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "OBJLoader.h"
class OBJmodel {
private:
	int vertNum;
	std::vector<Vertex> vertices;
	
	
	public:
	Vertex* data_ptr;
	OBJmodel();
	OBJmodel(const char* filePath);
	OBJmodel(const OBJmodel&);
	~OBJmodel();
	int getVertNum();
	glm::vec3 getVertPos(int vertNum);
	glm::vec3 getVertNormal(int vertNum);
	void applyMaterial(int matID);
	OBJmodel& merge(OBJmodel m);
	unsigned int getSizeOf();


};