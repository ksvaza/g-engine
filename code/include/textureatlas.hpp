#pragma once
#include <stdint.h>
#include <texture.hpp>

namespace Gengine
{
    class TextureAtlas
    {
    public:
        unsigned int textureID = 0U;

        unsigned char* data = NULL;
        int width, height, nrChannels;

        Texture** textures = NULL;
        void* textureBounds = NULL;
        void** elementReference = NULL;
        uint16_t textureCount = 0;

        int AddTexture(Texture* texture);
        int Bake(const char* filepath);

        static TextureAtlas Empty();
    };
    class TextGenerator
    {
    
    };
}