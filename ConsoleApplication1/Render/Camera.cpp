#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera()
{
	fieldofview = 45.0f;
	angle_h = 3.14f;
	angle_v = 0.0f;
}

Camera::~Camera()
{

}

void Camera::Update()
{
	/* Convert spherical angle coordinates to cartesian */
    glm::vec3 direction(cos(angle_v) * sin(angle_h), 
        sin(angle_v),
        cos(angle_v) * cos(angle_h)
    );

    // Right vector
    glm::vec3 right = glm::vec3(sin(angle_h - 3.14f/2.0f), 0, cos(angle_h - 3.14f/2.0f));

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    projection_matrix = glm::perspective(fieldofview, 16.0f / 9.0f, 0.1f, 4000.0f);

    // Set view matrix to look from camera position, to position + direction (look infront of camera)
   view_matrix = glm::lookAt(position, position+direction, up);
}
