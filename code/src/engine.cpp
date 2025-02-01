#include "../include/engine.hpp"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <winlib.hpp>
#include <hwinputs.hpp>
#include <render.hpp>
#include <mesh.hpp>
#include <shader.hpp>
#include <laygui.hpp>

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
        Shader shader;
        shader.Read("shaders/UIshader_vertex.glsl", GL_VERTEX_SHADER);
        shader.Read("shaders/UIshader_fragment.glsl", GL_FRAGMENT_SHADER);
        shader.Compile();
        shader.Use();

        G_UIelement element;
        {
            Transform meshTransform;
            meshTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
            meshTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            meshTransform.scale = glm::vec3(0.4f, 0.3f, 1.0f);

            Mesh mesh;
            MeshGen.RegularShape(&mesh, G_RECTANGLE);
            MeshGen.CalculateBounds(&mesh);
            mesh.SetTransform(meshTransform);
            mesh.SetColour(glm::vec4(0.2f, 0.7f, 0.4f, 1.0f));

            Transform elementTransform;
            elementTransform.position = glm::vec3(-0.8f, -0.85f, 0.0f);
            elementTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            elementTransform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

            Layout.CreateElement(&element, G_MESH);
            element.mesh = mesh;
            element.transform = elementTransform;
        }
        Layout.AddElement(element);
        G_UIelement* elementPtr = Layout.GetElementByUniqueID(element.uniqueID);

        G_UIelement base;
        {
            Layout.CreateElement(&base, G_PARENT);
            base.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
            base.transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            base.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
            base.mesh = Mesh::Empty();

            G_UIelement child;

            Mesh mesh;
            MeshGen.RegularShape(&mesh, G_HEXAGON);
            MeshGen.CalculateBounds(&mesh);
            mesh.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
            mesh.transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            mesh.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
            mesh.SetColour(glm::vec4(0.7f, 0.2f, 0.4f, 1.0f));

            Layout.CreateElement(&child, G_MESH);
            child.mesh = mesh;
            // Setting this transform has no effect, seems to be ignored, the layout system seems to only use the mesh transform
            child.transform.position = glm::vec3(0.0f, 0.25f, 0.0f);
            child.transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            child.transform.scale = glm::vec3(0.5f, 0.5f, 1.0f);

            Layout.AddChild(&base, &child);
        }
        Layout.AddElement(base);
        G_UIelement* basePtr = Layout.GetElementByUniqueID(base.uniqueID);
        
        TotalTime = 0.0f;
        float deltaTime = 0.0f;
        while (!glfwWindowShouldClose(window))
        {
            glfwSetTime((double)0.0);

            Update(deltaTime);

            Render.Clear();
            Input.Update();

            // Korrigieren die Mesh aus Mouse Position
            float x = ((Input.Mouse.MousePosition.x - (Gwindow.Width / 2)) * 2) / (Gwindow.Width);
            float y = -((Input.Mouse.MousePosition.y - (Gwindow.Height / 2)) * 2) / (Gwindow.Height);
            basePtr->transform.position = glm::vec3(x, y, 0.0f);
            basePtr->transform.rotation = glm::vec3(0.0f, 0.0f, 90.0f);

            Layout.DrawElements(Render, shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
            
            TotalTime += deltaTime;
            deltaTime = (float)glfwGetTime();
        }

        // The code for termination is suspiciously slow, probably due to the recursive deletion of elements
        // crashes instead of terminating
        shader.Delete();
        Layout.RemoveElement(element.uniqueID);
        Layout.RemoveElement(base.uniqueID);

        // This does not print
        printf("Window closed after %f seconds\n", TotalTime);

        return 0;
    }
}
