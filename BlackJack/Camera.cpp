#include "Camera.h"

Camera::Camera() {
	camY = 0.0f;
	camZ = 3.0f;

	cameraPos = glm::vec3(0.0, camY, camZ);
	glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
	cameraFront = glm::normalize(target - cameraPos);
	glm::vec3 right{ glm::vec3(1.0, 0.0, 0.0) };
	cameraUp = glm::normalize(glm::cross(right, cameraFront));
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	rotation = 0;
	rotating = false;
	rotDirection = 0;
	timer = 0;
}

void Camera::update(GLFWwindow* window, float dTime) {
	
	if (!rotating) {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && rotation < 0.5)
		{
			rotating = true;
			rotDirection = 1;
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && rotation > -0.5)
		{
			rotating = true;
			rotDirection = -1;
		}
	}
	else {
		float before = tween();
		timer += dTime;
		float after = tween();
		rotation += rotDirection * (after - before);
		camY = sin(rotation) * 3;
		camZ = cos(rotation) * 3;
	}
;}

float Camera::tween() {
	if (timer >= 1)
	{
		timer = 0;
		rotating = false;
		rotDirection = 0;
	}
	return(sqrt(1 - pow(timer - 1, 2)));
}

glm::mat4 Camera::getCamera() {
	cameraPos = glm::vec3(0.0, camY, camZ);
	glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
	cameraFront = glm::normalize(target - cameraPos);
	glm::vec3 right{ glm::vec3(1.0, 0.0, 0.0) };
	cameraUp = glm::normalize(glm::cross(right, cameraFront));
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return view;
}