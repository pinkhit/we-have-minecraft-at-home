#include "transform.h"

void transform::setPos(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void transform::setScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

// ang in degree, rmb to apply glm::radians if needed
void transform::setRot(float ang)
{
	rotation = ang;
}

const glm::mat4 transform::getModelMat(void)
{
	glm::mat4 model(1.0f);
	
	model = glm::translate(model, pos);
	//model = glm::rotate(model, glm::radians(rotation), axisOfRot)
	model = glm::scale(model, scale);
	return model;
}
