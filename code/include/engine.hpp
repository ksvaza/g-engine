#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/winlib.hpp"
#include "../include/hwinputs.hpp"

namespace Gengine
{
    class Engine
    {
    private:
        GLFWwindow* window;
        HWInputs Input;
        WindowInitializer Initialiser;
        int Update(float deltaTime);
    public:
        void Initialise();
        void Terminate();
        int Run();
    };
}
