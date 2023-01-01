#include "VAO.h"

Vao::Vao() {
	glGenVertexArrays(1, &ID);

}

void Vao::Generate() {
	glGenVertexArrays(1, &ID);

}

void Vao::LinkVBO(Vbo VBO, GLuint layout, GLint size) {
	VBO.Bind();
	glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();

}

void Vao::LinkVBO(Vbo* VBO, GLuint layout, GLint size, unsigned int stride) {
	VBO->Bind();
	glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(layout);
	VBO->Unbind();

}

void Vao::Bind() 
{
	glBindVertexArray(ID);
}

void Vao::Unbind()
{
	glBindVertexArray(0);
}

void Vao::Delete() {
	
	glDeleteVertexArrays(1, &ID);
}