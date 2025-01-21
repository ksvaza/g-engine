#pragma once
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowInitializer
{
public:
    static int InitialiseWindow(GLFWwindow** window, int width, int height, const char* name)
    {
        // GLFW initialisation
        if (!glfwInit())
        {
            printf("\n! Failed to initialise glfw !\n\n");
            return -1;
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Creating GLFW window
        *window = glfwCreateWindow(width, height, name, NULL, NULL);
        if (*window == NULL)
        {
            printf("Failed to create GLFW window\n");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(*window);

        // set default resizing function
        glfwSetFramebufferSizeCallback(*window, default_framebuffer_size_callback);

        // enable vsync
        glfwSwapInterval(1);

        // Initialise GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            printf("Failed to initialize GLAD\n");
            return -1;
        }

        glViewport(0, 0, width, height);

        return 0;
    }
    static int Terminate()
    {
        glfwTerminate();
        return 0;
    }
private:
    static void default_framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};
