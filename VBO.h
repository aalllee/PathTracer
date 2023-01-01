#pragma once
#include<glad/glad.h>
#include "Vertex.h"
#include <vector>
#include "OBJmodel.h"
class Vbo 
{

public: 
	unsigned int ID;
	Vbo();
	Vbo(GLsizeiptr size, std::vector<OBJmodel> models);
	Vbo(GLfloat* vertData, GLsizeiptr size);
	Vbo(Vertex* vertData, GLsizeiptr size);
	void Bind();
	void Update(GLfloat* vertData, GLsizeiptr size);
	void Update(Vertex* vertData, GLsizeiptr size);
	void Unbind();
	void Delete();


};