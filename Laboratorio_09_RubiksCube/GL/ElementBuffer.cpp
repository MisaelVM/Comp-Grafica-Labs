#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(const void* data, unsigned int size) {
	glGenBuffers(1, &elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer() {
	glDeleteBuffers(1, &elementBufferID);
}

void ElementBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
}

void ElementBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
