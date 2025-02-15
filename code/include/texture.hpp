#pragma once

namespace Gengine
{
    class Texture
    {
    public:
        unsigned int textureID = 0U;
        int width, height, nrChannels;

        int Load(const char* path);
    };
}