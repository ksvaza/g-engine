#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <winlib.hpp>
#include <glm/glm.hpp>

char InputBuffer[256];

GLFWwindow* window;

int Update(float deltaTime = 0.0f)
{
    return 0;
}


int main() {
    printf("Hello, World!\n");

    // Initialisation
    WindowInitializer::InitialiseWindow(&window, 800, 600, "Hello, World!");

    // Main loop
    float deltaTime = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        glfwSetTime((double)0.0);
        
        Update(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        deltaTime = (float)glfwGetTime();
    }


    //fgets(InputBuffer, 256, stdin);

    // Termination
    WindowInitializer::Terminate();

    return 0;
}
