#pragma once
#include<glad/glad.h>

class EBO
{

public:
	unsigned int ID;
	EBO();
	EBO(GLuint* IndexData, GLsizeiptr size);

	void Bind();
	void Update(GLuint* IndexData, GLsizeiptr size);
	void Unbind();
	void Delete();


};