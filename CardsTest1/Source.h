#ifndef SOURCE_H
#define SOURCE_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

int main();
void init();
void update();
void draw();
void onclose();
#endif