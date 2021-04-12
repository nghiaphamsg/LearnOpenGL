#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x2.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SOIL2.h>

#include <iostream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shaders.h"

void frameBufferResizeCallback(GLFWwindow* window, int fbw, int fbh);
void processInput(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
    /* Init GLFW */
    if (!glfwInit())
    {
        std::cout << "GLFW init failed!" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    /* Setup GLFW window properties */
    /* OpenGL version 3.3 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    /* Core profile is no backwards compatibility */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Allow forward compatibility (Mac OS) */
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    /* Lock resize window */
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    /* Create a new window */
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Basic", NULL, NULL);
    if (mainWindow == nullptr)
    {
        std::cout << "GLFW window creation failed!!" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Get buffer size information */
    int buffWidth = 0;
    int buffHeight = 0;
    glfwSetFramebufferSizeCallback(mainWindow, frameBufferResizeCallback);

    /* Tell GLFW to make the context of our window the main context on the current thread */
    glfwMakeContextCurrent(mainWindow);

    /* Allow modern extension features */
    glewExperimental = GL_TRUE;

    /* Need window & OpenGL context */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW init failed!!!" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return -1;
    }

    /* Configure global OpenGL state */
    glEnable(GL_DEPTH_TEST);

    /* Build and compile our shader program
       Vertex shader */
    Shaders ourShader("4.5.shader.vs", "4.5.shader.fs");

    /* Set up vertex data (and buffer(s)) and configure vertex attributes */
    /* 36 vertices (6 faces * 2 triangles * 3 vertices each) */
    float vertices[] = {
         /* Store 4 vertices for the rectangle */
         // position        // texture coords
         //0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
         //0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
         //-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
         //-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,   // top left
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO, VAO, texture;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    /* Bind the Vertex Array Object first, then bind and set vertex buffer(s), 
       and then configure vertex attributes(s) */
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Set position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Set texture coord attribute */
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* Note that this is allowed, the call to glVertexAttribPointer registered VBO
       as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /* Load & Create a texture */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Load image */
    int width, height, nrChannels;
    unsigned char* data = stbi_load("C:/Workspace/Project/OpenGL/image/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    ourShader.use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    /* Render loop */
    while (!glfwWindowShouldClose(mainWindow))
    {
        /* Input event */
        processInput(mainWindow);

        /* Clear window */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Draw */
        glBindTexture(GL_TEXTURE_2D, texture);

        ourShader.use();

        // Camera/view transformation
        glm::mat4 view = glm::mat4(1.0f);
        float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        /* Using to render to during this render iteration
           and show it as output to the screen */
        glfwSwapBuffers(mainWindow);

        /* Checks if any events are triggered
           (like keyboard input or mouse movement events)
           updates the window state */
        glfwPollEvents();
    }

    /* De-allocate all resources */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void frameBufferResizeCallback(GLFWwindow* window, int fbw, int fbh)
{
    glViewport(0, 0, fbw, fbh);
}

void processInput(GLFWwindow* window)
{
    /* Check whether the user has pressed the escape key then close window */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    /* Press 1 for filled */
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /* Press 2 for wireframe */
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Press 3 for point */
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}