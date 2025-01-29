#include "../include/engine.hpp"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <winlib.hpp>
#include <hwinputs.hpp>
#include <render.hpp>
#include <mesh.hpp>
#include <shader.hpp>

namespace Gengine
{
    int Engine::Update(float deltaTime)
    {
        return 0;
    }
    void Engine::Initialise()
    {
        Gwindow.InitialiseWindow(&window, 600, 600, "Hello, Triangle!");
        Input.Initialise(window);
        Input.SetTestMode(0);
    }
    void Engine::Terminate()
    {
        Gwindow.Terminate();
    }
    int Engine::Run()
    {
        Mesh mesh;
        MeshGen.RegularShape(&mesh, G_HEXAGON);
        MeshGen.CalculateBounds(&mesh);
        mesh.SetTransform();
        mesh.FillColour(1.0f, 0.5f, 0.2f, 1.0f);
        mesh.Print();

        Shader shader;
        shader.Read("shaders/vertex.glsl", GL_VERTEX_SHADER);
        //printf("Vertex shader: %s\n", shader.vertexShader);
        shader.Read("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
        //printf("Fragment shader: %s\n", shader.fragmentShader);
        shader.Compile();
        shader.Use();

        TotalTime = 0.0f;
        float deltaTime = 0.0f;
        while (!glfwWindowShouldClose(window))
        {
            glfwSetTime((double)0.0);

            Update(deltaTime);

            Render.Clear();
            Input.Update();
            shader.Use();

            // Korrigieren die Mesh aus Mouse Position
            float x = ((Input.Mouse.MousePosition.x - (Gwindow.Width / 2)) * 2) / (Gwindow.Width);
            float y = -((Input.Mouse.MousePosition.y - (Gwindow.Height / 2)) * 2) / (Gwindow.Height);
            mesh.SetTransform((Transform){position: glm::vec3(x, y, 0.0f), scale: glm::vec3(1.0f, 1.0f, 1.0f)});
            Render.DrawMesh(mesh, 0, 0); // crashes after second iteration

            glfwSwapBuffers(window);
            glfwPollEvents();
            
            TotalTime += deltaTime;
            deltaTime = (float)glfwGetTime();
        }
        printf("Window closed after %f seconds\n", TotalTime);

        mesh.Delete();

        return 0;
    }
}
