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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

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

    /* Build and compile our shader program
       Vertex shader */
    Shaders ourShader("4.5.shader.vs", "4.5.shader.fs");

    /* Set up vertex data (and buffer(s)) and configure vertex attributes */
    /* Element buffer objects */
    float vertices[] = {
         /* Store 4 vertices for the rectangle */
         // position         // colors        // texture coords
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // top left
    };

    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    unsigned int VBO, VAO, EBO, texture;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    /* Bind the Vertex Array Object first, then bind and set vertex buffer(s), 
       and then configure vertex attributes(s) */
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* Set position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Set color attribute */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* Set texture coord attribute */
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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
    unsigned char* data = stbi_load("{your_path}/image/container.jpg", &width, &height, &nrChannels, 0);
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

    /* Render loop */
    while (!glfwWindowShouldClose(mainWindow))
    {
        /* Input event */
        processInput(mainWindow);

        /* Clear window */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw */
        glBindVertexArray(VAO);
        //glBindTexture(GL_TEXTURE_2D, texture);
        ourShader.use();

        // Update the uniform color
        //float timeValue = glfwGetTime();
        //float greenValue = sin(timeValue) / 2.0f + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUniform4f(vertexColorLocation, 1.0f, greenValue, 0.0f, 1.0f);

         // Draw rectangle
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

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
    glDeleteBuffers(1, &EBO);

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