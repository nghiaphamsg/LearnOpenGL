// OpenGL_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Core profile is no backwards compatibility */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Allow forward compatibility */
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    /* Create a new window */
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (mainWindow == nullptr)
    {
        std::cout << "GLFW window creation failed!!" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Get buffer size information */
    int buffWidth, buffHeight;
    glfwGetFramebufferSize(mainWindow, &buffWidth, &buffHeight);

    /* Get context for GLEW to use */
    glfwMakeContextCurrent(mainWindow);

    /* Allow modern extension features */
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW init failed!!!" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return -1;
    }

    /* Setup viewport sizes
       setup the size of the part we're drawing to on our window */
    glViewport(0, 0, buffWidth, buffHeight);

    while (!glfwWindowShouldClose(mainWindow))
    {
        /* Get + Handler user input event */
        glfwPollEvents();

        /* Clear window */
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }


    return 0;
}