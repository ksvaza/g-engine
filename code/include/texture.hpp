#pragma once

namespace Gengine
{
    class Texture
    {
    public:
        unsigned int textureID;
        int width, height, nrChannels;

        int Load(const char* path);
    };
}