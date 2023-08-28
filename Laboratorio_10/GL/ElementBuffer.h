#pragma once

#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

class ElementBuffer {
private:
	unsigned int elementBufferID;

public:
	ElementBuffer(const void* data, unsigned int size);
	~ElementBuffer();

	void bind() const;
	void unbind() const;
};

#endif
