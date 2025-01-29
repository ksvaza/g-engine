#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Gengine
{
    class Window
    {
    public:
        static int InitialiseWindow(GLFWwindow** window, int width, int height, const char* name);
        static int Terminate();
        static int Width;
        static int Height;
        static float AspectRatio;
    private:
        static void default_framebuffer_size_callback(GLFWwindow* window, int width, int height);
    };
}
