#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	void update(GLFWwindow* window, float dTime);
	glm::mat4 getCamera();
	glm::vec3 cameraPos;
private:
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::mat4 view;
	float camY;
	float camZ;
	float rotation;
	float timer;

	bool rotating;
	int rotDirection;

	float tween();
};

#endif