#include "block.h"

block::block(void)
{
	shader_ = shader("textured.vert", "textured.frag");
	texture_ = texture();
	texture_.load("mbrot.png");

	cube = cubeMesh();

	trans_ = transform();

	shader_.bind();

}

// texture atlas support
block::block(std::string type)
{
	shader_ = shader("textured.vert", "textured.frag");
	texture_ = texture();
	texture_.load("blocks.png");
	cube = cubeMesh(type);

	trans_ = transform();

	shader_.bind();

}

block::~block(void)
{
}

void block::setPos(float x, float y, float z)
{
	trans_.setPos(x, y, z);
}

void block::update(float dt)
{
}

void block::render(glm::mat4 proj, glm::mat4 view, bool debug)
{	
	shader_.setUniformMat4("model", trans_.getModelMat());
	shader_.setUniformMat4("view", view);
	shader_.setUniformMat4("projection", proj);

	texture_.bind();

	glBindVertexArray(cube.getVAO()); 
	
	if (debug)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
