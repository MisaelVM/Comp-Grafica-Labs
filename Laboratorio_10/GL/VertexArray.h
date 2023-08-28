#pragma once

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexBuffer.h"

class VertexArray {
private:
	unsigned int vertexArrayID;

public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& VBO, int layout);

	void bind() const;
	void unbind() const;
};

#endif
