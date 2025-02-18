#pragma once
#include <stdint.h>

namespace Gengine
{
    class Texture
    {
    public:
        unsigned int textureID = 0U;

        unsigned char* data = NULL;
        int width, height, nrChannels;

        void* atlas = NULL; 
        int16_t atlasIndex = -1;

        int LoadData(const char* data);
        void FreeData();
        int Load(const char* path);
    };
}