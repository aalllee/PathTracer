#include "SSBO.h"

SSBO::SSBO() {
	glGenBuffers(1, &ID);
}

void SSBO::bind() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
}
void SSBO::update(GLsizeiptr size, const void* data,unsigned int base) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, base, ID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
}
void unbind() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
}
