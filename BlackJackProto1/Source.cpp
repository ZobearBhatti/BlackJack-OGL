#include "Source.h"

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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TableTest", NULL, NULL);
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
    glEnable(GL_BLEND);
    glEnable(GL_STENCIL_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

    srand((unsigned int)time(NULL));

    tableShader.init("Shaders/tableShader.vs", "Shaders/tableShader.fs");
    glGenVertexArrays(1, &tableVAO);
    glGenBuffers(1, &tableVBO);
    glBindVertexArray(tableVAO);
    glBindBuffer(GL_ARRAY_BUFFER, tableVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tableVertices), tableVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    cardShader.init("Shaders/cardShader.vs", "Shaders/cardShader.fs");
    cardShader.use();
    cardShader.setVec2("size", cardSize);
    glGenVertexArrays(1, &cardVAO);
    glGenBuffers(1, &cardVBO);
    glBindVertexArray(cardVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cardVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cardVertices), cardVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &cardTexture);
    glBindTexture(GL_TEXTURE_2D, cardTexture);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Textures/Cards.png", &width, &height,
        &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    addCard(true);
    addCard(false);

    glClearColor(0.2, 0.2, 0.2, 1.0);

    initTextRend();
}

void update() {
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera.update(window, deltaTime);

    lightPos.x = sin(currentFrame * 0.3f);

    if (playing)
    {
        if (!cardTaking) {
            if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
                cardTaking = true;
                animating = true;
                animPlayer = true;
            }
            else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
                if (dealerScore < 17 && dealerScore < playerScore) {
                    cardTaking = true;
                    dealerTimer = 0.4f;
                    tryStanding = true;
                }
                else {
                    evalWin(2);
                }
            }
        }
        else {
            if (dealerScore < 17) {
                dealerTimer += deltaTime;
                if (dealerTimer > 0.4f)
                {
                    if (!tryStanding) {
                        cardTaking = false;
                    }
                    dealerTimer = 0.0f;
                    animating = true;
                    animPlayer = false;
                }
            }
            else if (standing) {
                evalWin(2);
            }
        }

        if (animating) {
            animTimer += 5 * deltaTime;
            if (animTimer > 0.5) {
                animTimer = 0.0; animating = false;
                if (animPlayer) {
                    addCard(true);
                    playerScore = handValue(true);
                    if (playerScore > 21) {
                        evalWin(0);
                        cardTaking = false;
                    }
                    else if (playerScore == 21)
                    {
                        evalWin(2);
                    }
                    if (dealerScore >= 17 || dealerScore >= playerScore) { cardTaking = false; }
                }
                else {
                    addCard(false);
                    if (tryStanding) {
                        standing = true;
                    }
                    dealerScore = handValue(false);
                    if (dealerScore > 21) {
                        evalWin(1);
                    }
                    else if (dealerScore == 21) {
                        evalWin(0);
                    }
                }
            }
        }
    }
    else {
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            clearHands();
            addCard(true);
            addCard(false);
            playing = true;
            playerScore = 0;
            dealerScore = 0;
            cardTaking = false;
            tryStanding = false;
            standing = false;
        }
    }
}
void evalWin(int state) {
    /*states:
    * 0 - player bust
    * 1 - dealer bust
    * 2 - regular
    */
    playing = false;

    if (state == 0) {
        winner = 1;
        // dealer win
    }
    else if (state == 1) {
        // player win
        winner = 0;
    }
    else {
        if (playerScore > dealerScore) {
            // player win
            winner = 0;
        }
        else {
            // dealer win
            winner = 1;
        }
    }
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    drawTable();
    drawCards();
    drawScores();
}

void drawTable() {
    glBindVertexArray(tableVAO);
    tableShader.use();
    tableShader.setVec3("viewPos", camera.cameraPos);
    tableShader.setVec3("lightColor", glm::vec3(0.7));
    tableShader.setVec3("lightPos", lightPos);
    glm::mat4 trans = projection * view;
    tableShader.setMat4("transformation", trans);

    view = camera.getCamera();
    tableShader.setVec3("objectColor", glm::vec3(0.0, 0.5, 0.0));

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    tableShader.setVec3("objectColor", glm::vec3(0.5, 0.3, 0.0));

    for (int i = 1; i < 10; i++)
    {
        int j = i * 4;
        glDrawArrays(GL_TRIANGLE_FAN, j, 4);
    }
}

void drawCards() {
    cardShader.use();
    glBindVertexArray(cardVAO);
    glBindTexture(GL_TEXTURE_2D, cardTexture);

    float i = (0.0 - (playerCards.size() / 2.0f));
    for (int c : playerCards) {
        int value = c / 4;
        int face = c % 4;

        glm::mat4 model{ glm::mat4(1.0) };

        model = glm::translate(model, glm::vec3(0.0, -2.0, 0.0));
        model = glm::rotate(model, glm::radians(float(i)), glm::vec3(0.0, 0.0, -1.0));
        if (animPlayer) {
            model = glm::rotate(model, glm::radians(float(-animTimer)), glm::vec3(0.0, 0.0, -1.0));
            model = glm::translate(model, glm::vec3(-animTimer * 0.1f, 0.0, 0.0));
        }
        model = glm::translate(model, glm::vec3(i * 0.1f + 0.06, 1.6, 0.001 * i + 0.01));

        cardShader.setMat4("transformation", projection * view * model);

        cardShader.setVec2("position", glm::vec2((value) * 68.0f / 885.0f,
            (face) * 98.0f / 392.0f));

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        i += 1.0;
    }

    i = (0.0 - (dealerCards.size() / 2.0f));
    for (int c : dealerCards) {
        int value = c / 4;
        int face = c % 4;

        glm::mat4 model{ glm::mat4(1.0) };
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));

        model = glm::translate(model, glm::vec3(0.0, -2.0, 0.0));
        model = glm::rotate(model, glm::radians(float(i)), glm::vec3(0.0, 0.0, -1.0));
        if (!animPlayer) {
            model = glm::rotate(model, glm::radians(float(-animTimer)), glm::vec3(0.0, 0.0, -1.0));
            model = glm::translate(model, glm::vec3(-animTimer * 0.1f, 0.0, 0.0));
        }
        model = glm::translate(model, glm::vec3(i * 0.1f + 0.06, 1.6, 0.001 * i + 0.01));

        cardShader.setMat4("transformation", projection * view * model);

        cardShader.setVec2("position", glm::vec2((value) * 68.0f / 885.0f,
            (face) * 98.0f / 392.0f));

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        i += 1.0;
    }

}

void drawScores() {

    std::string pSc = std::to_string(playerScore);
    std::string dSc = std::to_string(dealerScore);

    RenderText(pSc, 220.0f, 720.0f, 1.0f,
        glm::vec3(0.0, 0.0, 1.0));

    RenderText(dSc, 1300.0f, 720.0f, 1.0f,
        glm::vec3(1.0, 0.0, 0.0));

    if (!playing) {
        std::string w = (winner == 0) ? "Player Wins" : "Dealer Wins";
        RenderText(w, 700, 800, 1.0, glm::vec3(1.0, 1.0, 0.0));
    }

}

void addCard(bool player) {
    if (player) {
        playerCards.push_back(newCard());
    }
    else {
        dealerCards.push_back(newCard());
    }
}

int newCard() {
    int face = std::rand() % 4;
    int value = std::rand() % 13;

    return(face + (value * 4));
}

void clearHands() {
    playerCards.clear();
    dealerCards.clear();
}

int handValue(bool player) {
    int handValue{ 0 };
    int i = 0;
    int end;
    if (player) {
        end = playerCards.size();
        playerCards.sort();
        playerCards.reverse();

        for (int c : playerCards) {
            int value = c / 4;
            if (value > 8) {
                handValue += 10;
            }
            else {
                handValue += value + 1;
            }
            if (value == 0) {
                if (handValue + (end - i - 1) + 10 <= 21) {
                    handValue += 10;
                }
            }
            i++;
        }
    }
    else {
        end = dealerCards.size();
        dealerCards.sort();
        dealerCards.reverse();

        for (int c : dealerCards) {
            int value = c / 4;
            if (value > 8) {
                handValue += 10;
            }
            else {
                handValue += value + 1;
            }
            if (value == 0) {
                if (handValue + (end - i - 1) + 10 <= 21) {
                    handValue += 10;
                }
            }
            i++;
        }
    }
    std::cout << handValue << "\n";
    return handValue;
}

void onclose() {
    glDeleteVertexArrays(1, &tableVAO);
    glDeleteBuffers(1, &tableVBO);
}

