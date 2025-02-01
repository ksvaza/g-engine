#pragma once
#include <mesh.hpp>
#include <shader.hpp>

namespace Gengine
{
    class Renderer
    {
    private:
        
    public:
        int DrawMesh(Mesh mesh, char wireFrame, char boundingBox, Shader shader);
        void Clear();
    };
}
