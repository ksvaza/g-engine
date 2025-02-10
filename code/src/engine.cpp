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
#include <uicreator.hpp>

namespace Gengine
{
    int Engine::Update(float deltaTime)
    {
        return 0;
    }
    void Engine::Initialise()
    {
        Gwindow.InitialiseWindow(&window, 864, 486, "Hello, Triangle!");
        Input.Initialise(window, &Gwindow);
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

        Layout.SetUIshader(shader);
        Layout.SetInput(&Input);
        Layout.SetWindow(&Gwindow);
        glm::mat4 viewMatrix = glm::mat4(1.0);
        glm::mat4 projectionMatrix = OrthographicMatrix(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
        Layout.SetUIviewMatrix(viewMatrix);
        Layout.SetUIprojectionMatrix(projectionMatrix);

        // UI window
        // G_UIelement wElement;
        // G_UIelement bElement;
        // G_UIelement cElement;
        // G_UIelementAttribute* wElementButton;
        // {
        //     Mesh wMesh;
        //     MeshGen.RegularShape(&wMesh, G_RECTANGLE);
        //     wMesh.transform = NewTransform(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(400.0, 600.0, 1.0));
        //     wMesh.SetColour(glm::vec4(0.2, 0.2, 0.2, 1.0));

        //     Layout.CreateElement(&wElement, G_MESH);
        //     wElement.mesh = wMesh;
        //     wElement.transform = NewTransform(glm::vec3(960, 540, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));


        //     Mesh bMesh;
        //     MeshGen.RegularShape(&bMesh, G_RECTANGLE);
        //     bMesh.transform = NewTransform(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(380.0, 30.0, 1.0));
        //     bMesh.SetColour(glm::vec4(0.4, 0.4, 0.4, 1.0));

        //     Layout.CreateButton(&bElement);
        //     Layout.AddButtonCallbacks(&bElement, Layout.DefaultButtonStateChange, Layout.DefaultButtonHoverIn, Layout.DefaultButtonHoverOut, Layout.DefaultButtonPress, Layout.DefaultButtonRelease);
        //     bElement.mesh = bMesh;
        //     bElement.transform = NewTransform(glm::vec3(0.0, 275.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
        //     wElementButton = Layout.GetAttributeByType(&bElement, G_BUTTON_ATTRIB);
        //     Layout.AddChild(&wElement, &bElement);
        //     AABox bBounds = Layout.CalculateRelativeBounds(&bElement, -1);
        //     Layout.AddButtonBounds(&bElement, bBounds);


        //     Mesh cMesh;
        //     MeshGen.RegularShape(&cMesh, G_RECTANGLE);
        //     cMesh.transform = NewTransform(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(380.0, 530.0, 1.0));
        //     cMesh.SetColour(glm::vec4(0.9, 0.9, 0.9, 1.0));

        //     Layout.CreateElement(&cElement, G_MESH);
        //     cElement.mesh = cMesh;
        //     cElement.transform = NewTransform(glm::vec3(0.0, -25.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
        //     Layout.AddChild(&wElement, &cElement);
        // }
        // Layout.AddElement(&wElement);
        G_UIelement wElement;
        G_UIelementAttribute* wElementButton;
        Layout.CreateUIWindow(&wElement, &wElementButton, glm::vec2(960.0, 540.0), glm::vec2(1200.0, 800.0), 2.0, 50.0);
        Layout.AddUIWindowColours(&wElement, glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(0.2, 0.2, 0.2, 1.0), glm::vec4(0.7, 0.75, 0.8, 1.0));
        Layout.AddElement(&wElement);

        G_UIelement element;
        G_UIelementAttribute* elementButton;
        {
            Mesh mesh;
            MeshGen.RegularShape(&mesh, G_RECTANGLE);
            mesh.transform.position = glm::vec3(0.0, 0.0, 0.0);
            mesh.transform.rotation = glm::vec3(0.0, 0.0, 0.0);
            mesh.transform.scale = glm::vec3(400, 300, 1.0);
            mesh.SetColour(glm::vec4(0.2, 0.7, 0.4, 1.0));

            Layout.CreateButton(&element);
            Layout.AddButtonCallbacks(&element, Layout.DefaultButtonStateChange, Layout.DefaultButtonHoverIn, Layout.DefaultButtonHoverOut, Layout.DefaultButtonPress, Layout.DefaultButtonRelease);
            element.mesh = mesh;
            element.transform.position = glm::vec3(200, 100, 0.0);
            element.transform.rotation = glm::vec3(0.0, 0.0, 0.0);
            element.transform.scale = glm::vec3(1.0, 1.0, 1.0);
            AABox bounds = Layout.CalculateRelativeBounds(&element, -1);
            Layout.AddButtonBounds(&element, bounds);
            elementButton = Layout.GetAttributeByType(&element, G_BUTTON_ATTRIB);
        }
        Layout.AddElement(&element);

        G_UIelement base;
        {
            Layout.CreateElement(&base, G_PARENT);
            base.visible = 1;
            base.transform.position = glm::vec3(0.0, 0.0, 0.0);
            base.transform.rotation = glm::vec3(0.0, 0.0, 0.0);
            base.transform.scale = glm::vec3(1.0, 1.0, 1.0);
            base.mesh = Mesh::Empty();

            G_UIelement child;

            Mesh mesh;
            MeshGen.RegularShape(&mesh, G_HEXAGON);
            mesh.transform.position = glm::vec3(0.0, 0.0, 0.0);
            mesh.transform.rotation = glm::vec3(0.0, 0.0, 0.0);
            mesh.transform.scale = glm::vec3(200.0, 200.0, 1.0);
            mesh.SetColour(glm::vec4(0.7, 0.2, 0.4, 1.0));

            Layout.CreateElement(&child, G_MESH);
            child.mesh = mesh;
            child.transform.position = glm::vec3(960.0, 540.0, 0.0);
            child.transform.rotation = glm::vec3(0.0, 0.0, 0.0);
            child.transform.scale = glm::vec3(1.0, 1.0, 1.0);

            Layout.AddChild(&base, &child);
        }
        Layout.AddElement(&base);

        GUI_slider slider;
        slider.Create(0.0, 1.0, GUI_HORIZONTAL, glm::vec2(200.0, 20.0), glm::vec2(20.0, 20.0), glm::vec2(960.0, 540.0), glm::vec4(0.2, 0.2, 0.2, 1.0), glm::vec4(0.7, 0.75, 0.8, 1.0));
        slider.SetReferenceLayout(&Layout);
        slider.AddToLayout();

        Layout.Compile();

        //wElement.supermesh.Print();
        
        TotalTime = 0.0f;
        float deltaTime = 0.0f;
        while (!glfwWindowShouldClose(window))
        {
            glfwSetTime((double)0.0);

            Update(deltaTime);

            Render.Clear();
            Layout.Update();
            Input.Update();

            // Korrigieren die Mesh aus Mouse Position
            glm::vec3 deltaMouseWorldPos = glm::vec3(Input.ConvertPixelToWorldSpace(Input.Mouse.MouseDeltaPosition, glm::vec2(Gwindow.Width, Gwindow.Height), viewMatrix, projectionMatrix), 0.0);
            glm::vec3 mouseWorldPos = glm::vec3(Input.ConvertPixelToWorldSpace(Input.Mouse.MousePosition, glm::vec2(Gwindow.Width, Gwindow.Height), viewMatrix, projectionMatrix), 0.0);
            //printf("Mouse world position: %f, %f\n", deltaMouseWorldPos.x, deltaMouseWorldPos.y);
            if (elementButton->button.isPressed)
            {
                element.transform.position += deltaMouseWorldPos;
            }
            if (wElementButton->button.isPressed)
            {
                wElement.transform.position += deltaMouseWorldPos;
            }
            Layout.DrawElements();

            glfwSwapBuffers(window);
            glfwPollEvents();
            
            TotalTime += deltaTime;
            deltaTime = (float)glfwGetTime();
        }

        // The code for termination is suspiciously slow, probably due to the recursive deletion of elements // -- fixed
        // crashes instead of terminating // -- fixed
        shader.Delete();
        Layout.RemoveElement(&element);
        Layout.DeleteElement(&element);
        Layout.RemoveElement(&base);
        Layout.DeleteElement(&base);
        Layout.RemoveElement(&wElement);
        Layout.DeleteElement(&wElement);

        // This does not print // -- fixed
        printf("Window closed after %f seconds\n", TotalTime);

        return 0;
    }
}
