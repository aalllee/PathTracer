#pragma once
#include <glad/glad.h>
#include "VBO.h"

class Vao {
public:
	GLuint ID;
	Vao();
	void Generate();
	void LinkVBO(Vbo VBO, GLuint layout, GLint size);
	void LinkVBO(Vbo* VBO, GLuint layout, GLint size, unsigned int stride);
	void Bind();
	void Unbind();
	void Delete();


};