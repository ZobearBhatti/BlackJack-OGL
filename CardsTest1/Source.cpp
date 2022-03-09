#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Source.h"

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"

#include <windows.h>
#include <iostream>

GLFWwindow* window;
float currentFrame;
float deltaTime;
float lastFrame;
const unsigned int SCR_WIDTH = 1066;
const unsigned int SCR_HEIGHT = 600;

const glm::mat4 projection{ glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, -100.0f) };
//const glm::mat4 projection{ glm::ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0) };
glm::mat4 view;
glm::mat4 model{ glm::mat4(1.0f) };

#pragma region VARIABLES

Shader txShader;
Shader stShader;

unsigned int cardVAO, cardVBO, cardTexture;

float cardVertices[]{
    -5.0, -5.0, 3.0,      0.0, 1.0, // tl
    5.0, -5.0, 3.0,       1.0, 1.0,   // tr
    5.0, 5.0, 1.0,        1.0, 0.0,      // br
    -5.0, 5.0, 1.0,       0.0, 0.0    // bl
};

#pragma endregion

int main()
{
    SetProcessDPIAware();

    // INITIALISE OGL
        // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CamTest", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    init();

    while (!glfwWindowShouldClose(window))
    {
        update();
        draw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    onclose();

    glfwTerminate();
    return 0;
}

void init() {
    stShader.init("Shaders/shader.vs", "Shaders/shader.fs");
    txShader.init("Shaders/texShader.vs", "Shaders/texShader.fs");

    glGenVertexArrays(1, &cardVAO);
    glGenBuffers(1, &cardVBO);

    glBindVertexArray(cardVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cardVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cardVertices), cardVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    glGenTextures(1, &cardTexture);
    glBindTexture(GL_TEXTURE_2D, cardTexture);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Textures/Card.png", &width, &height,
        &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 3.0);
    glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 cameraFront = glm::normalize(target - cameraPos);
    glm::vec3 right{ glm::vec3(1.0, 0.0, 0.0) };
    glm::vec3 cameraUp = glm::normalize(glm::cross(right, cameraFront));
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    model = glm::translate(model, glm::vec3(0.0, 0.0, -20.0));

    glClearColor(0.3, 0.5, 0.2, 1.0);
}

void update() {
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(cardVAO);
    txShader.use();
    glBindTexture(GL_TEXTURE_2D, cardTexture);

    glm::mat4 trans{ projection * view * model };
    txShader.setMat4("transformation", trans);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void onclose() {

}