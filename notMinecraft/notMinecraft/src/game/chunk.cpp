#include "chunk.h"

constexpr int SIZE = 64;

chunk::chunk()
{
	// init chunk data as all air
	data = new int[SIZE * SIZE * SIZE]();

	shader_ = new shader("textured.vert", "textured.frag");
	texture_ = new texture();
	texture_->load("blocks.png");

	// bind shader & texture once since wont change
	shader_->bind();
	texture_->bind();

	blockMeshes_[types::GRASS - 1] = new cubeMesh("grass");
	blockMeshes_[types::DIRT - 1] = new cubeMesh("dirt");
	blockMeshes_[types::STONE - 1] = new cubeMesh("stone");
	blockMeshes_[types::WOOD - 1] = new cubeMesh("wood");
}

chunk::~chunk()
{
	delete[] data;
}

void chunk::generateTerrain(std::vector<float> heights)
{
	// set surface layer
	for (int x = 0; x < SIZE; x++)
		for (int z = 0; z < SIZE; z++)
		{
			int surfaceAt = heights[z * SIZE + x] * SIZE;
			if (surfaceAt < 0)
				surfaceAt = 0;
			// todo:: can implement threshhold for chunkY to set minimum height
			setBlock(x, surfaceAt, z, types::GRASS);

			// set underground block types
			int chunkY = surfaceAt - 1;
			while (chunkY >= 0)
			{
				// block within 2 blocks below: grass
				if((surfaceAt - chunkY) <= 2)
					setBlock(x, chunkY, z, types::DIRT);
				else
					setBlock(x, chunkY, z, types::STONE);
				chunkY--;
			}
		}
}

void chunk::render(glm::mat4 proj, glm::mat4 view)
{
	shader_->setUniformMat4("view", view);
	shader_->setUniformMat4("projection", proj);

	for (int x = 0; x < SIZE; x++)
		for (int y = 0; y < SIZE; y++)
			for (int z = 0; z < SIZE; z++)
			{
				int type = getBlock(x, y, z);
				if (type == types::AIR) continue;

				transform trans_ = transform();
				trans_.setPos(x, y, z);
				shader_->setUniformMat4("model", trans_.getModelMat());

				glBindVertexArray(blockMeshes_[type - 1]->getVAO());
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
}

void chunk::setBlock(int x, int y, int z, types bType)
{
	int index = (x * SIZE * SIZE) + (y * SIZE) + z;
	data[index] = bType;
}

int chunk::getBlock(int x, int y, int z)
{
	int index = (x * SIZE * SIZE) + (y * SIZE) + z;
	return data[index];
}


void chunk::print(void) const
{
	for (int y = 0; y < SIZE; y++)
	{
		std::cout << "---row: " << y << " ---" << std::endl;
		for (int x = 0; x < SIZE; x++)
		{
			std::cout << "x: " << x << " ||";
			for (int z = 0; z < SIZE; z++)
			{
				int index = (x * SIZE * SIZE) + (y * SIZE) + z;
				std::cout << " z: " << z << " [" << data[index] << "] |";
			}
			std::cout << std::endl;
		}
	}
}


