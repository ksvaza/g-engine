#include "../include/engine.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <winlib.hpp>
#include <hwinputs.hpp>
//#include <render.hpp>
#include <sprite.hpp>

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
        Input.SetTestMode(0);
    }
    void Engine::Terminate()
    {
        Initialiser.Terminate();
    }
    int Engine::Run()
    {
        Sprite sprite;
        sprite.Default();

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
