#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <winlib.hpp>
#include <hwinputs.hpp>
#include <render.hpp>
#include <mesh.hpp>

namespace Gengine
{
    class Engine
    {
    private:
        GLFWwindow* window;
        HWInputs Input;
        Window Gwindow;
        Renderer Render;
        MeshGenerator MeshGen;
        int Update(float deltaTime);
    public:
        float TotalTime;
        void Initialise();
        void Terminate();
        int Run();
    };
}
