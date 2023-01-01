#pragma once
#include<glad/glad.h>
class SSBO {
public:
	unsigned int ID;

	SSBO();

	void bind();
	void update(GLsizeiptr size, const void* data,unsigned int base);
	void unbind();


};