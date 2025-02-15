#pragma once
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Gengine
{
    class Shader
    {
    private:
        char* vertexShader = NULL;
        char* fragmentShader = NULL;
        unsigned int vertexShaderID = 0;
        unsigned int fragmentShaderID = 0;
        unsigned int shaderProgram = 0;
    public:
        int Read(const char* filepath, GLenum type); // GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
        int Compile();
        unsigned int Use();
        void Delete();

        void SetUniformMat4(const char* name, glm::mat4 matrix);
        void SetUniformVec4(const char* name, glm::vec4 vector);
        void SetUniform1iv(const char* name, int count, int* values);

        Shader(){};
        ~Shader(){};
    };
}