#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <winlib.hpp>
#include <hwinputs.hpp>
#include <render.hpp>

namespace Gengine
{
    class Engine
    {
    private:
        GLFWwindow* window;
        HWInputs Input;
        WindowInitializer Initialiser;
        Renderer Render;
        int Update(float deltaTime);
    public:
        float TotalTime;
        void Initialise();
        void Terminate();
        int Run();
    };
}
