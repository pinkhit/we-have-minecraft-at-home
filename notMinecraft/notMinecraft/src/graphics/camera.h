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

	void yaw(float angle);
	void pitch(float angle);
	void roll(float angle);

	void updateLook(float yawAng, float pitchAng);

	void normalizeVectors(void);

	bool mouse_leftEdge = false;
	bool mouse_rightEdge = false;
	bool mouse_bottEdge = false;
	bool mouse_TopEdge = false;

private:
	glm::vec4 eye;
	glm::vec4 backV;
	glm::vec4 upV;
	glm::vec4 global_upV;
	glm::vec4 rightV;

	float vp_width;
	float vp_height;
	float vp_distance;

	float near;
	float far;
	float fov;
	float aspectRatio;
};