#pragma once

#include "../components/physics.h"
#include "../components/transform.h"
#include "../graphics/cubeMesh.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"

// individual block entity
class block
{
public:
	block(void);
	block(std::string);
	~block(void);

	void setPos(float x, float y, float z);

	void update(float dt);
	void render(glm::mat4 proj, glm::mat4 view, bool debug);

private:
	shader shader_;
	texture texture_;
	cubeMesh cube;
	physics* pPhys;
	transform trans_;
};