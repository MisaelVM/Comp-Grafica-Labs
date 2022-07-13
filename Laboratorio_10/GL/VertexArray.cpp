#include "VertexArray.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &vertexArrayID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vertexArrayID);
}

void VertexArray::addBuffer(const VertexBuffer& VBO, int layout) {
	VBO.bind();
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(layout + 1);
	glVertexAttribPointer(layout + 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));
	VBO.unbind();
}

void VertexArray::bind() const {
	glBindVertexArray(vertexArrayID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}
