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

void frameBufferResizeCallback(GLFWwindow* window, int fbw, int fbh);
void processInput(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Triangle Window", NULL, NULL);
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
    //glfwGetFramebufferSize(mainWindow, &buffWidth, &buffHeight);

    /* Setup viewport sizes
       setup the size of the part we're drawing to on our window */
    //glViewport(0, 0, buffWidth, buffHeight);

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
       vertex shader */
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    /* Check for shader compile errors */
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    /* Fragment shader */
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
   
    /* Check for shader compile errors */
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /* Link shaders */
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    /* Check for linking errors */
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /* Set up vertex data (and buffer(s)) and configure vertex attributes */
    //float vertices[] = {
    //    -0.5f, -0.5f, 0.0f, // left
    //     0.5f, -0.5f, 0.0f, // right
    //     0.0f,  0.5f, 0.0f  // top
    //};

    /* Element buffer objects */
    float vertices[] = {
         /* Store 4 vertices for the rectangle */
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,  // top left
    };

    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    unsigned int VBO, VAO, EBO;
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

    /* Set the vertex attributes pointers */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Note that this is allowed, the call to glVertexAttribPointer registered VBO
       as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /* Render loop */
    while (!glfwWindowShouldClose(mainWindow))
    {
        /* Input event */
        processInput(mainWindow);

        /* Clear window */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        /* Draw */
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Draw triangles
        //glDrawArrays(GL_TRIANGLES, 0, 3);

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
    glDeleteProgram(shaderProgram);

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