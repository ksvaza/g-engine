#pragma once
#include <mesh.hpp>

namespace Gengine
{
    class Renderer
    {
    private:
        
    public:
        int DrawMesh(Mesh mesh, char wireFrame, char boundingBox);
        void Clear();
    };
}
