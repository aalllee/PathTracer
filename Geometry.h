#pragma once
#include "VBO.h"
#include "Shader.h"
#include "OBJmodel.h"
#include "Material.h"
class Geometry {
public:
	Geometry();
	Geometry(const char* model_filePath,const char* shader_filePath);
	Geometry(OBJmodel* model, const char* shader_filePath);
	~Geometry();


	void applyMaterial(int matID);

	OBJmodel* model;
	Vbo* vertexBufferObject;
	Shader* shader;

};