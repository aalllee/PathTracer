#include "VBO.h"

Vbo::Vbo() {

}

Vbo::Vbo(GLfloat* vertData, GLsizeiptr size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertData, GL_STATIC_DRAW);

}

Vbo::Vbo(Vertex* vertData, GLsizeiptr size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertData, GL_STATIC_DRAW);

}




void Vbo::Update(GLfloat* vertData, GLsizeiptr size) {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertData, GL_STATIC_DRAW);

}

void Vbo::Update(Vertex* vertData, GLsizeiptr size) {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertData, GL_STATIC_DRAW);

}



void Vbo::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void Vbo::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Vbo::Delete() {
    glDeleteBuffers(1, &ID);
}