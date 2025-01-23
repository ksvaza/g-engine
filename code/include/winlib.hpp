#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Gengine
{
    class WindowInitializer
    {
    public:
        static int InitialiseWindow(GLFWwindow** window, int width, int height, const char* name);
        static int Terminate();
    private:
        static void default_framebuffer_size_callback(GLFWwindow* window, int width, int height);
    };
}
