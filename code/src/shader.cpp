#include "../include/shader.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace Gengine {
    void Shader::Delete()
    {
        free(vertexShader);
        free(fragmentShader);
    }
    int Shader::Read(const char* filepath, GLenum type)
    {
        // Open file
        FILE* file = fopen(filepath, "r");
        if (file == NULL)
        {
            printf("Failed to open file: %s\n", filepath);
            return 1;
        }

        // Get file length
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Write to correct buffer and setup source
        if (type == GL_VERTEX_SHADER)
        {
            vertexShader = (char*)malloc(length + 1);
            memset(vertexShader, 0, length + 1);
            fread(vertexShader, 1, length, file);
        }
        else if (type == GL_FRAGMENT_SHADER)
        {
            fragmentShader = (char*)malloc(length + 1);
            memset(fragmentShader, 0, length + 1);
            fread(fragmentShader, 1, length, file);
        }
        else
        {
            printf("Invalid shader type: %d\n", type);
            return 2;
        }
        fclose(file);

        return 0;
    }
    int Shader::Compile()
    {
        glDeleteShader(vertexShaderID);
        vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderID, 1, &vertexShader, NULL);
        
        glCompileShader(vertexShaderID);
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
            printf("Vertex shader compilation failed: %s\n", infoLog);
            return 1;
        }

        glDeleteShader(fragmentShaderID);
        unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderID, 1, &fragmentShader, NULL);
        
        glCompileShader(fragmentShaderID);
        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
            printf("Fragment shader compilation failed: %s\n", infoLog);
            return 2;
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShaderID);
        glAttachShader(shaderProgram, fragmentShaderID);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            printf("Shader program linking failed: %s\n", infoLog);
            return 3;
        }

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        return 0;
    }
    unsigned int Shader::Use()
    {
        glUseProgram(shaderProgram);
        return shaderProgram;
    }
    
    void Shader::SetUniformMat4(const char* name, glm::mat4 matrix)
    {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void Shader::SetUniformVec4(const char* name, glm::vec4 vector)
    {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniform4fv(location, 1, glm::value_ptr(vector));
    }
    void Shader::SetUniformVec2(const char* name, glm::vec2 vector)
    {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniform2fv(location, 1, glm::value_ptr(vector));
    }
    void Shader::SetUniform1i(const char* name, int value)
    {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniform1i(location, value);
    }
    void Shader::SetUniform1iv(const char* name, int count, int* values)
    {
        unsigned int location = glGetUniformLocation(shaderProgram, name);
        glUniform1iv(location, count, values);
    }
}
