#ifndef SOURCE_H
#define SOURCE_H  

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "shader.h"
#include "stb_image.h"

#include <windows.h>
#include <list>
#include <iterator>


#include <cstdlib>
#include <ctime>

int main();
void init();
void update();
void draw();
void drawTable();
void drawCards();

void onclose();

GLFWwindow* window;
bool cardTaking = false;
float dealerTimer;
bool animating = false;
bool animPlayer;
bool tryStanding = false;
bool standing = false;
float animTimer;
float currentFrame;
float deltaTime;
float lastFrame;
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

bool playing = true;

const glm::mat4 projection{ glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f) };
glm::mat4 view;
Camera camera = Camera();

#pragma region VARIABLES

unsigned int tableVAO, tableVBO;
const float tableVertices[]{
    // top face
    -1.0f, -0.7f, 0.0f, 0.0, 0.0, 1.0,
    1.0f, -0.7f, 0.0f, 0.0, 0.0, 1.0,
    1.0f, 0.7f, 0.0f, 0.0, 0.0, 1.0,
    -1.0f, 0.7f, 0.0f, 0.0, 0.0, 1.0,

    // oiqt
    -1.0f, -0.7f, 0.0f, 0.0, 1.0, 1.0,
    1.0f, -0.7f, 0.0f, 0.0, 1.0, 1.0,
    1.1f, -0.8f, 0.1f, 0.0, 1.0, 1.0,
    -1.1f, -0.8f, 0.1f, 0.0, 1.0, 1.0,

    // oiql
    -1.0f, -0.7f, 0.0f, 1.0, 0.0, 1.0,
    -1.0f, 0.7f, 0.0f, 1.0, 0.0, 1.0,
    -1.1f, 0.8f, 0.1f, 1.0, 0.0, 1.0,
    -1.1f, -0.8f, 0.1f, 1.0, 0.0, 1.0,

    // oiqr
    1.0f, -0.7f, 0.0f, -1.0, 0.0, 1.0,
    1.0f, 0.7f, 0.0f, -1.0, 0.0, 1.0,
    1.1f, 0.8f, 0.1f, -1.0, 0.0, 1.0,
    1.1f, -0.8f, 0.1f, -1.0, 0.0, 1.0,

    // oiqb
    1.0f, 0.7f, 0.0f, 0.0, -1.0, 1.0,
    -1.0f, 0.7f, 0.0f, 0.0, -1.0, 1.0,
    -1.1f, 0.8f, 0.1f, 0.0, -1.0, 1.0,
    1.1f, 0.8f, 0.1f, 0.0, -1.0, 1.0,

    // ooqt
    -1.1f, -0.8f, 0.1f, 0.0, -1.0, 0.0,
    1.1f, -0.8f, 0.1f, 0.0, -1.0, 0.0,
    1.1f, -0.8f, -0.2f, 0.0, -1.0, 0.0,
    -1.1f, -0.8f, -0.2f, 0.0, -1.0, 0.0,

    // ooql
    -1.1f, -0.8f, 0.1f, -1.0, 0.0, 0.0,
    -1.1f, 0.8f, 0.1f, -1.0, 0.0, 0.0,
    -1.1f, 0.8f, -0.2f, -1.0, 0.0, 0.0,
    -1.1f, -0.8f, -0.2f, -1.0, 0.0, 0.0,

    // ooqr
    1.1f, -0.8f, 0.1f, 1.0, 0.0, 0.0,
    1.1f, 0.8f, 0.1f, 1.0, 0.0, 0.0,
    1.1f, 0.8f, -0.2f, 1.0, 0.0, 0.0,
    1.1f, -0.8f, -0.2f, 1.0, 0.0, 0.0,

    // ooqb
     1.1f, 0.8f, 0.1f, 0.0, 1.0, 0.0,
    -1.1f, 0.8f, 0.1f, 0.0, 1.0, 0.0,
    -1.1f, 0.8f, -0.2f, 0.0, 1.0, 0.0,
    1.1f, 0.8f, -0.2f, 0.0, 1.0, 0.0,

    // bottom face
    -1.1f, -0.8f, -0.2f, 0.0, 0.0, -1.0,
    1.1f, -0.8f, -0.2f, 0.0, 0.0, -1.0,
    1.1f, 0.8f, -0.2f, 0.0, 0.0, -1.0,
    -1.1f, 0.8f, -0.2f, 0.0, 0.0, -1.0,
};
Shader tableShader;
// card vertex information on plane with z 0.5, between x (-1.0, 1.0) and y (-1.0, 1.0)

Shader cardShader;
unsigned int cardTexture;
unsigned int cardVAO, cardVBO;
float cardVertices[]{
    -0.1, -0.15, 0.0, 0.0, 1.0,
    0.1, -0.15, 0.0, 1.0, 1.0,
    0.1, 0.15, 0.0, 1.0, 0.0,
    -0.1, 0.15, 0.0, 0.0, 0.0,
};

glm::vec2 cardSize{ glm::vec2(67.0f / 885.0f, 97.0f / 392.0f) };
glm::vec3 lightPos{ glm::vec3(0.0, 0.5, 2.0) };

std::list<int> playerCards;
std::list<int> dealerCards;

void addCard(bool player);
int newCard();
void clearHands();
int handValue(bool player);
int playerScore;
int dealerScore;
void evalWin(int state);

#pragma endregion
#endif