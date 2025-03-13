#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <winlib.hpp>
#include <hwinputs.hpp>
#include <render.hpp>
#include <mesh.hpp>
#include <laygui.hpp>
#include <game.hpp>

namespace Gengine
{
    class Engine
    {
    private:
        GLFWwindow* window;
        HWInputs Input;
        Window Gwindow;
        Renderer Render;
        MeshGenerator MeshGen;
        Glayout Layout;
        GameWorld Game;
        int Update(float deltaTime);
    public:
        float TotalTime;
        void Initialise();
        void Terminate();
        int Run();
    };
}
