#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
private:
	unsigned int textureID;
	unsigned char* textureData;
	int width, height, nrChannels;

public:
	Texture();
	Texture(const std::string& texturePath);
	~Texture();

	void create(const std::string& texturePath);
	void bind(unsigned int textureUnit = 0) const;
	void unbind() const;
};

#endif
