#pragma once
#include <stdint.h>

namespace Gengine
{
    class Texture
    {
    public:
        unsigned int textureID = 0U;

        unsigned char* data = NULL;
        int width = 0, height = 0, nrChannels = 0;

        void* atlas = NULL; 
        int16_t atlasIndex = -1;

        int LoadData(const char* data);
        void FreeData();
        int Load(const char* path);
        static Texture Empty();
    };
}