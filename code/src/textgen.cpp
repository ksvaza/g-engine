#include "../include/textgen.hpp"
#include <stdio.h>
#include <string.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Gengine
{
    int TextureAtlas::AddTexture(Texture texture)
    {
        textures = (Texture*)realloc(textures, sizeof(Texture) * (textureCount + 1));
        texture.atlas = this;
        textures[textureCount] = texture;
        textureCount++;
        return 0;
    }
    int TextureAtlas::Bake(const char* filepath)
    {
        if (textureCount < 1)
        {
            printf("No textures to bake\n");
            return -1;
        }

        int16_t maxHeight = 0;
        int32_t totalWidth = 0;
        for (int i = 0; i < textureCount; i++)
        {
            if (textures[i].height > maxHeight)
            {
                maxHeight = textures[i].height;
            }
            totalWidth += textures[i].width;
        }
        data = (unsigned char*)malloc(maxHeight * totalWidth * 4);
        printf("Total width: %d, total height: %d, textureCount: %d\n", totalWidth, maxHeight, textureCount);

        int32_t xOffset = 0;
        for (int y = 0; y < maxHeight; y++)
        {
            for (uint16_t i = 0; i < textureCount; i++)
            {
                if (y >= textures[i].height)
                {
                    xOffset += textures[i].width;
                    memset(data + 4 * (xOffset + y * totalWidth), 0, textures[i].width * 4);
                    continue;
                }
                memcpy(data + 4 * (xOffset + y * totalWidth), textures[i].data + y * textures[i].width * 4, textures[i].width * 4);
                xOffset += textures[i].width;
            }
        }
        printf("Here\n");
        stbi_write_bmp(filepath, totalWidth, maxHeight, 4, data);
        free(data);


        /*int64_t totalDataSize = 0;
        double totalArea = 0.0;
        for (int i = 0; i < textureCount; i++)
        {
            totalDataSize += textures[i].width * textures[i].height * textures[i].nrChannels;
            totalArea += textures[i].width * textures[i].height;
        }
        data = (unsigned char*)malloc(totalDataSize);
        intptr_t dataOffset = 0;
        for (int i = 0; i < textureCount; i++)
        {
            memcpy(data + dataOffset, textures[i].data, textures[i].width * textures[i].height * textures[i].nrChannels);
            dataOffset += textures[i].width * textures[i].height * textures[i].nrChannels;
        }
        stbi_write_bmp(filepath, textures[0].width, 2.0 * textures[0].height, 4, data);*/
        return 0;
    }
}