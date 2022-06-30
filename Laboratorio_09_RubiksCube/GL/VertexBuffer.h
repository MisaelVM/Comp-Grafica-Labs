#pragma once

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "glad/glad.h"

class VertexBuffer {
private:
	unsigned int vertexBufferID;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};

#endif
