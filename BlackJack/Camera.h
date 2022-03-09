#pragma once
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
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::mat4 view;
	float camY;
	float camZ;
	float rotation;
	float timer;
	float rotTimer;

	bool rotating;
	int rotDirection;


	float tween();
};

