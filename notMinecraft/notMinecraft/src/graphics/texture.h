#pragma once

#include <string>

static const std::string texturePath = "data/assets/Textures/";

static std::vector<std::pair<std::string, unsigned int>> textureLib;

class texture
{
public:
	texture();
	~texture();

	bool load(std::string);

	void bind();
	void unbind();

private:
	std::string textureSource;
	unsigned int ID;    // ID of texture object
	unsigned char* imageData; // to store data loaded in from stb_image

	int numColorChannels;
	int width;
	int height;
};