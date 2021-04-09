#ifndef __SHADERS_H__
#define __SHADERS_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shaders
{
public:
	unsigned int ID;

    /* Constructor generates the shader on the fly */
	Shaders(const char* vertexPath, const char* fragmentPath)
	{
        /* 1. retrieve the vertex/fragment source code from filePath */
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        /* Ensure ifstream objects can throw exceptions */
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            /* Open files */
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            /* Read file's buffer contents into streams */
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            /* Close file handlers */
            vShaderFile.close();
            fShaderFile.close();

            /* Convert stream into string */
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        /* 2. compile shaders */
        unsigned int vertex, fragment;
        /* Vertex shader */
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        /* Fragment Shader */
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        /* Shader Program */
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        /* Delete the shaders as they're linked into our program now and no longer necessary */
        glDeleteShader(vertex);
        glDeleteShader(fragment);
	}

	/* Activate the shader */
	void use()
	{
		glUseProgram(ID);
	}

    /* Set  */
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

	~Shaders()
	{
        glDeleteProgram(ID);
	}

private:
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: "
                          << type << "\n" << infoLog
                          << "\n ------------------"
                          << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: "
                          << type << "\n" << infoLog 
                          << "\n ------------------" 
                          << std::endl;
            }
        }
    }
};

#endif	/* __SHADERS_H__ */
