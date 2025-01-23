#include "../include/engine.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/winlib.hpp"
#include "../include/hwinputs.hpp"

namespace Gengine
{
    int Engine::Update(float deltaTime)
    {
        return 0;
    }
    void Engine::Initialise()
    {
        Initialiser.InitialiseWindow(&window, 800, 600, "Hello, World!");
        Input.Initialise(window);
        Input.SetTestMode(1);
    }
    void Engine::Terminate()
    {
        Initialiser.Terminate();
    }
    int Engine::Run()
    {
        float deltaTime = 0.0f;
        while (!glfwWindowShouldClose(window))
        {
            glfwSetTime((double)0.0);
            
            Update(deltaTime);

            Input.Update();   

            glfwSwapBuffers(window);
            glfwPollEvents();
            
            deltaTime = (float)glfwGetTime();
        }

        return 0;
    }
}
