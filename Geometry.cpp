#include "Geometry.h"
		
Geometry::Geometry() {
	model = nullptr;
	vertexBufferObject = nullptr;
	shader = nullptr;

}

Geometry::~Geometry() {
	delete shader;
	delete vertexBufferObject;
	delete model;

}

Geometry::Geometry(const char* model_filePath, const char* shader_filePath) {
	model = new OBJmodel(model_filePath);
	shader = new Shader(shader_filePath);
	vertexBufferObject = new Vbo(model->data_ptr, model->getSizeOf());
}

Geometry::Geometry(OBJmodel* model, const char* shader_filePath) {
	this->model = new OBJmodel(*model);
	shader = new Shader(shader_filePath);
	vertexBufferObject = new Vbo(this->model->data_ptr, this->model->getSizeOf());
}




void Geometry::applyMaterial(int matID) {

	

}


