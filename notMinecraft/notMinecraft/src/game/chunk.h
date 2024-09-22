#pragma once

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "components/transform.h"
#include "graphics/cubeMesh.h"

enum types
{
	AIR,
	GRASS,
	DIRT,
	STONE,
	WOOD
};

class chunk
{
public:
	chunk();
	~chunk();

	void generateTerrain(std::vector<float>);

	void render(glm::mat4 p, glm::mat4 v);

	void setBlock(int, int, int, types);

	int getBlock(int, int, int);

	void print(void) const;
private:
	shader* shader_;
	texture* texture_;
	// default chunk size is 16
	int* data;

	cubeMesh* blockMeshes_[4] = {};
};
