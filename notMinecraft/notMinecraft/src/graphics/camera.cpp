#include "camera.h"

camera::camera(void)
{
	// center of projection is the origin
	eye = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	// right, up, and back vectors are parallel to the x, y, and z axes respectively
	rightV = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	upV = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	global_upV = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	backV = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

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

	// update auto look based on if mouse in margin
	if (this->mouse_bottEdge)
		updateLook(0.0f, -0.015f);
	if (this->mouse_TopEdge)
		updateLook(0.0f, 0.015f);
	if (this->mouse_rightEdge)
		updateLook(0.015f, 0.0f);
	if (this->mouse_leftEdge)
		updateLook(-0.015f, 0.0f);
}

glm::mat4 camera::getViewMat()
{
	// global up +y axis
	glm::mat4 view = glm::lookAt(glm::vec3(eye), glm::vec3(eye) - glm::vec3(backV), glm::vec3(global_upV));
	return view;
}

glm::mat4 camera::getProjMat()
{
	glm::mat4 projection = glm::perspective(fov, aspectRatio, near, far);
	return projection;
}

void camera::forward(float distance)
{
	this->normalizeVectors();
	eye -= distance * backV;
}

void camera::right(float distance)
{
	this->normalizeVectors();
	eye += distance * rightV;
}

void camera::up(float distance)
{
	this->normalizeVectors();
	eye += distance * global_upV;
}

// BUG- was not ensuring orthogonality preserved: read and understand then delete
//void camera::yaw(float angle)
//{
//	upV = glm::normalize(upV);
//	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(upV));
//	rightV = glm::normalize(rot * rightV);
//	backV = glm::normalize(rot * backV);
//}
// 
//void camera::pitch(float angle)
//{
//	rightV = glm::normalize(rightV);
//	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rightV));
//	upV = glm::normalize(rot * upV);
//	backV = glm::normalize(rot * backV);
//}
//
//void camera::roll(float angle)
//{
//	backV = glm::normalize(backV);
//	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(backV));
//	rightV = glm::normalize(rot * rightV);
//	upV = glm::normalize(rot * upV);
//}
//
//void camera::updateLook(float yawAng, float pitchAng)
//{
//	this->yaw(yawAng);
//	this->pitch(pitchAng);
//	
//
//	// potential bug: movement abit wonky??? right and up vectors are weird but front is fine
//}
//
//void camera::normalizeVectors()
//{
//	// Ensure backV is normalized
//	backV = glm::normalize(backV);
//
//	// ensure orthogonality
//	glm::vec3 bXu = glm::cross(glm::vec3(upV), glm::vec3(backV));
//	rightV = glm::vec4(glm::normalize(bXu), 1.0f);
//
//	glm::vec3 rXb = glm::cross(glm::vec3(backV), glm::vec3(rightV));
//	upV = glm::vec4(glm::normalize(rXb), 1.0f);
//}

void camera::normalizeVectors() 
{
	// Ensure the vectors remain perpendicular to each other (re-orthogonalization)
	backV = glm::normalize(backV);
	glm::vec3 bXu = glm::cross(glm::vec3(global_upV), glm::vec3(backV));
	rightV = glm::vec4(glm::normalize(bXu), 1.0f);
	glm::vec3 bXr = glm::cross(glm::vec3(backV), glm::vec3(rightV));
	upV = glm::vec4(glm::normalize(bXr), 1.0f);
}

void camera::yaw(float angle) 
{
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(global_upV));
	backV = glm::vec4(glm::normalize(glm::vec3(rot * backV)), 1.0f);
	// Recalculate rightV to ensure orthogonality
	glm::vec3 bXu = glm::cross(glm::vec3(global_upV), glm::vec3(backV));
	rightV = glm::vec4(glm::normalize(bXu), 1.0f);
}

void camera::pitch(float angle) 
{
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rightV));
	backV = glm::vec4(glm::normalize(glm::vec3(rot * backV)),1.0f);
	glm::vec3 bXr = glm::cross(glm::vec3(backV), glm::vec3(rightV));
	upV = glm::vec4(glm::normalize(bXr), 1.0f);
}

void camera::roll(float angle) 
{
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(backV));
	rightV = glm::vec4(glm::normalize(glm::vec3(rot * rightV)), 1.0f);
	glm::vec3 bXr = glm::cross(glm::vec3(backV), glm::vec3(rightV));
	upV = glm::vec4(glm::normalize(bXr), 1.0f);
}

void camera::updateLook(float yawAng, float pitchAng) {
	this->yaw(yawAng);
	this->pitch(pitchAng);
	this->normalizeVectors();  // Normalize and reorthogonalize vectors 
}