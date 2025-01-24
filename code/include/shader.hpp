#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Gengine
{
    class Shader
    {
    public:
        char* vertexShader;
        char* fragmentShader;
        unsigned int vertexShaderID, fragmentShaderID;
        unsigned int shaderProgram;
    //public:
        int Read(const char* filepath, GLenum type); // GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
        int Compile();
        unsigned int Use();
        Shader();
        ~Shader();
    };
}