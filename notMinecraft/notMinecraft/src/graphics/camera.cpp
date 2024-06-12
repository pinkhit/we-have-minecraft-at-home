#include "camera.h"

camera::camera(void)
{
	// center of projection is the origin
	eye = glm::vec3(0.0f, 0.0f, 0.0f);

	// right, up, and back vectors are parallel to the x, y, and z axes respectively
	rightV = glm::vec3(1.0f, 0.0f, 0.0f);
	upV = glm::vec3(0.0f, 1.0f, 0.0f);
	backV = glm::vec3(0.0f, 0.0f, 1.0f);

	// viewport has field of view of 90, aspect ratio of 1
	fov = 45.0f;
	vp_width = 800.0f;
	vp_height = 600.0f;
	aspectRatio = vp_width / vp_height;
	vp_distance = (vp_width / 2.0f) / tanf(glm::radians(fov) / 2.0f);

	// near clipping plane distance of 0.1
	near = 0.1f;
	// and a far clipping plane distance of 10 in world coordinates
	far = 100.0f;

}

camera::camera(float fov_, float near_, float far_, float width, float height)
{
}

camera::~camera()
{

}

void camera::update(float width, float height)
{
	vp_width = width;
	vp_height = height;
	vp_distance = (vp_width / 2.0f) / tanf(glm::radians(fov) / 2.0f);
	aspectRatio = width / height;
}

glm::mat4 camera::getViewMat()
{
	glm::mat4 view = glm::lookAt(eye, eye - backV, upV);
	return view;
}

glm::mat4 camera::getProjMat()
{
	glm::mat4 projection = glm::perspective(fov, aspectRatio, near, far);
	return projection;
}

void camera::forward(float distance)
{
	eye -= distance * backV;
}

void camera::right(float distance)
{
	eye += distance * rightV;
}

void camera::up(float distance)
{
	eye += distance * upV;
}
