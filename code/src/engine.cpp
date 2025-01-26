#include "../include/engine.hpp"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <winlib.hpp>
#include <hwinputs.hpp>
#include <render.hpp>
#include <sprite.hpp>
#include <shader.hpp>

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
        Shader shader;
        shader.Read("shaders/vertex.glsl", GL_VERTEX_SHADER);
        shader.Read("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
        printf("Fragment shader: %s\n", shader.fragmentShader);
        shader.Compile();
        shader.Use();

        TotalTime = 0.0f;
        float deltaTime = 0.0f;
        while (!glfwWindowShouldClose(window))
        {
            glfwSetTime((double)0.0);

            printf("Update, ");
            Update(deltaTime);

            printf("Clear, ");
            Render.Clear();
            printf("IOUpdate, ");
            Input.Update();
            printf("Use, ");
            shader.Use();
            printf("DrawSprite, ");
            Render.DrawSprite();//sprite); // crashes after second iteration

            printf("SwapBuffers, ");
            glfwSwapBuffers(window);
            printf("PollEvents\n");
            glfwPollEvents();
            
            TotalTime += deltaTime;
            deltaTime = (float)glfwGetTime();
        }
        printf("Window closed after %f seconds\n", TotalTime);

        return 0;
    }
}
