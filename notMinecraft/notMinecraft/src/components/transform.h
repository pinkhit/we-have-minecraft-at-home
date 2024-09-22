#pragma once


class transform
{
public:
	transform(void): pos(0), scale(1.0f), rotation(0) {}

	void setPos(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setRot(float);

	
	// send model mat to shader
	const glm::mat4 getModelMat(void);

private:
	glm::vec3 pos;
	glm::vec3 scale;
	float rotation;
};