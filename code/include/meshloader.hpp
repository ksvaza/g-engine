#pragma once
#include <mesh.hpp>

namespace Gengine
{
    class MeshLoader
    {
    public:
        static int LoadOBJ(Mesh* mesh, const char* filepath);
        static int LoadMTL(Mesh* mesh, const char* filepath);
    };
}