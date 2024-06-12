#pragma once


class camera
{
public:
	camera(void); // canonical cam
	camera(float fov_, float near_, float far_, float width, float height);
	~camera();

	void update(float, float);

	glm::mat4 getViewMat();
	glm::mat4 getProjMat();

	void forward(float);
	void right(float);
	void up(float);

private:
	glm::vec3 eye;
	glm::vec3 backV;
	glm::vec3 upV;
	glm::vec3 rightV;

	float vp_width;
	float vp_height;
	float vp_distance;

	float near;
	float far;
	float fov;
	float aspectRatio;
};