#include "OBJmodel.h"
OBJmodel::OBJmodel() {
	vertices = { };
	vertNum = 0;
	data_ptr = nullptr;

}
OBJmodel::OBJmodel(const char* file_path){
	vertices = loadOBJ(file_path);
	vertNum = vertices.size();
	data_ptr = vertices.data();
};

//copy const
OBJmodel::OBJmodel(const OBJmodel &model) {
	this->vertices = model.vertices;
	this->vertNum = model.vertNum;
	this->data_ptr = this->vertices.data();

}

OBJmodel::~OBJmodel() {
	//delete [] data_ptr;
}

int OBJmodel::getVertNum() {
	return vertNum;
}

void OBJmodel::applyMaterial(int matID) {

	for (int i = 0; i < vertices.size(); i++) {
	
		vertices[i].color.x = matID;
	
	}
	
}


glm::vec3 OBJmodel::getVertPos(int vertNum) {
	
	return vertices[vertNum].position;
}

glm::vec3 OBJmodel::getVertNormal(int vertNum) {

	return vertices[vertNum].normal;
}

unsigned int OBJmodel::getSizeOf() {
	return getVertNum() * sizeof(Vertex);

}

OBJmodel& OBJmodel::merge(OBJmodel m) {
	
	this->vertices.insert(this->vertices.end(), m.vertices.begin(), m.vertices.end());
	vertNum = this->vertices.size();
	data_ptr = vertices.data();

	return *this;
}